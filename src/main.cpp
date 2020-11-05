
#include "mbed.h"

InterrCimatIn button(p10);
DigitalOut LedVermelho(p11);
DigitalOut LedAzul(p12);
PwmOut Intensidade(p13);

Timeout timeout;
Ticker ticker;

enum STATE
{
  Baixo, Cima, Descer, MIN, Subir, MAX
};

void init();
void PressButton();
void ReleaseButton();
void transitionStatePress();
void ledBlink();

int EstadoAtual;
bool stateButton;
float Frequencia;

int main() {
    init();
    button.Subir(&PressButton);
    button.Descer(&ReleaseButton);
}

void transitionStatePress(){
    timeout.attach(&ledBlink, 0.1);
    if(stateButton){
        switch (EstadoAtual){
            case Cima:
                EstadoAtual = Subir;
                printf("Subir\n");
                break;
            case Baixo:
                EstadoAtual = Descer;
                printf("Descer\n");
                break;
            case Descer:
                if(Intensidade==0.0){
                    EstadoAtual = MIN;
                    printf("MIN\n");
                }else{
                    Intensidade = Intensidade - 0.05;
                    printf("Intensidade: %.2f\n",Intensidade.read());
                }
                break;
            case Subir:
                if(Intensidade==1.0){
                    EstadoAtual = MAX;
                    printf("MAX\n");
                }else{
                    Intensidade = Intensidade + 0.05;
                    printf("Intensidade: %.2f\n",Intensidade.read());
                }
                break;
            default:
                if (Intensidade >= 1.0) {
                    EstadoAtual = MAX;
                    printf("MAX\n");
                } else if (Intensidade <= 0.0) {
                        EstadoAtual = MIN;
                        printf("MIN\n");
                }
                break;
        }
    }
}

void init(){
    LedAzul = 1.0;
    LedVermelho = 0.0;
    Intensidade = 1.0;
    EstadoAtual = Cima;
    printf("Cima\n");
}

void PressButton()
{
    printf("Press Button\n");
    stateButton = true;
    ticker.attach(&transitionStatePress,1.0);
}

void ReleaseButton()
{
    printf("Release Button\n");
    stateButton = false;
    ticker.detach();
    if(EstadoAtual == MAX || EstadoAtual == Cima || EstadoAtual == Descer){
        if(Intensidade > 0.0){
            LedAzul = 0.0;
            LedVermelho = 1.0;
            EstadoAtual = Baixo;
            printf("Baixo\n");
        }
    }
    else if(EstadoAtual == MIN || EstadoAtual == Baixo || EstadoAtual == Subir){
        if(Intensidade < 1.0){
            LedAzul = 1.0;
            LedVermelho = 0.0;
            EstadoAtual = Cima;
            printf("Cima\n");
        }
    }
}

void ledBlink(){
    switch(EstadoAtual){
        case Descer:
            Frequencia = 1;
            LedVermelho = !LedVermelho;
            break;
        case Subir:
            Frequencia = 1;
            LedAzul = !LedAzul;
            break;
        case MIN:
            Frequencia = 0.1;
            LedVermelho = !LedVermelho;
            break;
        case MAX:
            Frequencia = 0.1;
            LedAzul = !LedAzul;
            break;
        default:
            break;
    }
    timeout.attach(&ledBlink, Frequencia);
}
