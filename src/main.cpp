
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

int current_state;
bool stateButton;
float frequency;

int main() {
    init();
    button.Subir(&PressButton);
    button.Descer(&ReleaseButton);
}

void transitionStatePress(){
    timeout.attach(&ledBlink, 0.1);
    if(stateButton){
        switch (current_state){
            case Cima:
                current_state = Subir;
                printf("Subir\n");
                break;
            case Baixo:
                current_state = Descer;
                printf("Descer\n");
                break;
            case Descer:
                if(Intensidade==0.0){
                    current_state = MIN;
                    printf("MIN\n");
                }else{
                    Intensidade = Intensidade - 0.05;
                    printf("Intensidade: %.2f\n",Intensidade.read());
                }
                break;
            case Subir:
                if(Intensidade==1.0){
                    current_state = MAX;
                    printf("MAX\n");
                }else{
                    Intensidade = Intensidade + 0.05;
                    printf("Intensidade: %.2f\n",Intensidade.read());
                }
                break;
            default:
                if (Intensidade >= 1.0) {
                    current_state = MAX;
                    printf("MAX\n");
                } else if (Intensidade <= 0.0) {
                        current_state = MIN;
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
    current_state = Cima;
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
    if(current_state == MAX || current_state == Cima || current_state == Descer){
        if(Intensidade > 0.0){
            LedAzul = 0.0;
            LedVermelho = 1.0;
            current_state = Baixo;
            printf("Baixo\n");
        }
    }
    else if(current_state == MIN || current_state == Baixo || current_state == Subir){
        if(Intensidade < 1.0){
            LedAzul = 1.0;
            LedVermelho = 0.0;
            current_state = Cima;
            printf("Cima\n");
        }
    }
}

void ledBlink(){
    switch(current_state){
        case Descer:
            frequency = 1;
            LedVermelho = !LedVermelho;
            break;
        case Subir:
            frequency = 1;
            LedAzul = !LedAzul;
            break;
        case MIN:
            frequency = 0.1;
            LedVermelho = !LedVermelho;
            break;
        case MAX:
            frequency = 0.1;
            LedAzul = !LedAzul;
            break;
        default:
            break;
    }
    timeout.attach(&ledBlink, frequency);
}
