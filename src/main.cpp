BL_TrafficLight2


#include "mbed.h"

InterruptIn botao(p10);
DigitalOut LedVermelho(p11);
DigitalOut LedAmarelo(p12);
DigitalOut LedAzul(p13);

Timeout timeout;
Ticker ticker;

enum STATE
{
  OFF, ALERT, Vermelho, Amarelo , Azul, ON, START
};

int current_state;
int previe_state;
int manual_count;
int stateButton
;

void transitionState();
void Padrao();


void MudarAzul()
{
    printf("Azul\n");
    LedAzul = 1;
    previe_state = current_state;
    current_state = Azul;
    timeout.attach(&transitionState, 20);
}

void MudarAmarelo()
{
    printf("Amarelo\n");
    LedAmarelo = 1;
    previe_state = current_state;
    current_state = Amarelo;
    timeout.attach(&transitionState, 4);
}


void MudarVermelho()
{
    printf("Vermelho\n");
    LedVermelho = 1;
    previe_state = current_state;
    current_state = Vermelho;
    timeout.attach(&transitionState, 10);
}




void MudarAmarelo1Hz(){
    LedAmarelo = !LedAmarelo;
    ticker.attach(MudarAmarelo1Hz, 0.5);
}

void MudarAlerta(){
    printf("alert\n");
    previe_state = current_state;
    current_state = ALERT;
    MudarAmarelo1Hz();
}

void Desligar(){
    printf("off\n");
    previe_state = current_state;
    current_state = OFF;
    Padrao();
}

void manual_count_fun(){
    manual_count++;
}

void startCount()
{
    printf("Start count\n");
    timeout.detach();
    manual_count = 0;
    ticker.attach(manual_count_fun,1);
}

void endCount()
{
    printf("End count\n");
    ticker.detach();
    stateButton
 = ON;
    transitionState();
}

int main() {
    current_state = START;
    previe_state = START;
    transitionState();
    
    botao.rise(&startCount);
    botao.fall(&endCount);
    
    while(1) {
        wait(1);
        printf("botao Count %d\n",manual_count);
    }
}

void transitionState(){
    Padrao();
    if(current_state == START && previe_state == START){
        printf("Vermelho init\n");
        LedVermelho = 1;
        previe_state = Vermelho;
        current_state = Vermelho;
        timeout.attach(&transitionState, 10);
    }
    else if(stateButton == OFF){
        if(current_state == Vermelho){
            MudarAzul();
        }
        else if(current_state == Azul){
            MudarAmarelo();
        }
        else if(current_state == Amarelo){
            MudarVermelho();
        }
    }
    else if(stateButton == ON){
        stateButton = OFF;
        if(current_state == Vermelho){
            if(manual_count>=3 && manual_count<=10){
                MudarAlerta();
            }
            else if(manual_count > 10){
                Desligar();
            }
            else{
                MudarVermelho();
            }
        }
        else if(current_state == Amarelo){
            if(manual_count>=3 && manual_count<=10){
                MudarAlerta();
            }
            else if(manual_count > 10){
                Desligar();
            }
            else{
                MudarAmarelo();
            }
        }
        else if(current_state == Azul){
            if(manual_count>=3 && manual_count<=10){
                MudarAlerta();
            }
            else if(manual_count==20 || manual_count<=3){
                MudarAmarelo();
            }
            else if(manual_count>10){
                Desligar();
            }
            else{
                MudarAzul();
            }
        }
        else if(current_state == ALERT){
            if(manual_count>=3 && manual_count<=10){
                if(previe_state == Vermelho){
                    MudarVermelho();
                }
                else if(previe_state == Amarelo){
                    MudarAmarelo();
                }
                else{
                    MudarAzul();
                }
            }
            else if(manual_count > 10){
                Desligar();
            }
            else{
                current_state = previe_state;
                MudarAlerta();
            }
        }
        else if(current_state == OFF){
            if(manual_count > 10){
                MudarVermelho();
            }
            else{
                Desligar();
            }
        }
    }
}

void Padrao()
{
  LedVermelho = 0;
  LedAmarelo = 0;
  LedAzul = 0;
  
  timeout.detach();
  ticker.detach();
}
