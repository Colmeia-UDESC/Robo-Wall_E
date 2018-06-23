//---------------------------------------BIBLIOTECAS---------------------------------------

#include <AFMotor.h>
#include <Servo.h>
#include <Ultrasonic.h>

//-----------------------------------------SENSOR------------------------------------------

#define DISTANCIA_COLISAO 25                                    //Distancia para o carrinho parar ao ver um objeto é 25cm

#define TRIG_PINBACK A0
#define ECHO_PINBACK A1
Ultrasonic sensorTraseiro(TRIG_PINBACK, ECHO_PINBACK);          //Seta o sensor traseiro

#define TRIG_PINFRONT A2
#define ECHO_PINFRONT A3
Ultrasonic sensorFrontalInferior(TRIG_PINFRONT, ECHO_PINFRONT); //Seta o sensor frontal inferior

#define TRIG_PIN A4
#define ECHO_PIN A5
Ultrasonic sensorFrontalSuperior(TRIG_PIN, ECHO_PIN);           //Seta o sensor frontal superior
Servo servo;

int TESTE = 1;


//----------------------------------------MOTOR--------------------------------------------
 


AF_DCMotor motorEsquerdo(1);  //Cria motor 1 
AF_DCMotor motorDireito(2);   //Cria motor 2

//-----------------------------------------------------------------------------------------
//----------------------------------------SETUP--------------------------------------------
//-----------------------------------------------------------------------------------------

void setup(){
        Serial.begin(9600); 
  	Serial.println("Iniciando");
	servo.attach(9);  
	Serial.println("Servo 'setado' com sucesso");
        servo.write(90); //Com o servo no meio dizemos que aquela posicao eh a central, 90 (Em estudo ainda)
        motorEsquerdo.setSpeed(180); // Para essa velocidade de 180  necessario 0,5seg de delay para o carrinho girar 90 graus
        motorDireito.setSpeed(180); // Para essa velocidade de 180  necessario 0,5seg de delay para o carrinho girar 90 graus
        //servo.write(180);  faz o servo virar para a esquerda
        //servo.write(90);   faz o servo centralizar
        //servo.write(45);   faz o servo virar pra direita
        
	
}

//-----------------------------------------------------------------------------------------
//----------------------------------------LOOP---------------------------------------------
//-----------------------------------------------------------------------------------------

void loop(){
    avaliarDistanciaFrontal();
}

//-----------------------------------------------------------------------------------------
//---------------------------------------FUNCOES-------------------------------------------
//-----------------------------------------------------------------------------------------

void avaliarDistanciaFrontal(){      //Avaliar distancia sensor frontal superior e inferior
	int distanciaFrontalSuperior = leSensorFrontalSuperior();
	int distanciaFrontalInferior = leSensorFrontalInferior();

	if((distanciaFrontalSuperior <= DISTANCIA_COLISAO) || (distanciaFrontalInferior <= DISTANCIA_COLISAO)){
                //casos();
		casosTeste();
        }
        
        
	if((distanciaFrontalSuperior >= DISTANCIA_COLISAO) && (distanciaFrontalInferior >= DISTANCIA_COLISAO)){
		andar();
		TESTE = 1;
	}
}

void avaliarDistanciaTraseira(){     //Avaliar distancia sensor traseiro
	int distanciaTraseira = leSensorTraseiro();

	if(distanciaTraseira <= DISTANCIA_COLISAO)
		parar();
	else
		re();

	TESTE = 1;
	casos();
}

//---------------------------------------LEITURAS------------------------------------------

int leSensorFrontalSuperior(){                     //Le distancia sensor frontal superior
  	float cm = sensorFrontalSuperior.Ranging(CM);
  	Serial.print("Distancia do sensor frontal superior eh: ");
  	Serial.print(cm);
  	Serial.println("");
  	return cm;
}

int leSensorFrontalInferior(){                     //Le distancia sensor frontal inferior
  	float cm = sensorFrontalInferior.Ranging(CM);
  	Serial.print("Distancia do sensor frontal inferior eh: ");
  	Serial.print(cm);
  	Serial.println("");
  	return cm;
}

int leSensorTraseiro(){                            //Le distancia sensor traseiro
	float cm = sensorTraseiro.Ranging(CM);
	Serial.print("Distancia do sensor traseiro eh: ");
	Serial.print(cm);
	Serial.println("");
	return cm;
}

//--------------------------------------MOVIMENTOS-----------------------------------------

void virarDireita(){            //Virar carrinho para direita
    motorEsquerdo.run(FORWARD);
    motorDireito.run(BACKWARD);
    TESTE = 2;
    delay(500);    //-------Avaliar se é o suficiente para virar
    Serial.println("Virou Direita");
}

void virarEsquerda(){           //Virar carrinho para esquerda pelo dobro do tempo, pois esta virado para direita
    motorEsquerdo.run(BACKWARD);
    motorDireito.run(FORWARD);
    TESTE = 0;
    delay(500);    //-------Avaliar se é o suficiente para virar
    Serial.println("Virou Esquerda");
}

void parar(){                   //Para o carrinho
    motorEsquerdo.run(RELEASE);
    motorDireito.run(RELEASE);
    delay(70);
}

void re(){                      //Dar ré no carrinho
    motorEsquerdo.run(BACKWARD);
    motorDireito.run(BACKWARD);
    delay(500);
    Serial.println("Parou");
}

void andar(){                   //Mover o carrinho pra frente
    motorEsquerdo.run(FORWARD);
    motorDireito.run(FORWARD);
    delay(500);
    parar();                   // parar foi botado aqui para testes e assim no bater, dando tempo para a verificacao frontal
    Serial.println("Andou");
}

int verificaEsquerdaServo(){
   servo.write(185);
   int cme = leSensorFrontalSuperior();
   //delay(750);
   //servo.write(90);
   delay(500);
   Serial.print("Distancia esquerda eh: ");
   Serial.println (cme);
   delay(300);
   return cme;
}

int verificaDireitaServo(){
   servo.write(50);
   int cmd = leSensorFrontalSuperior();
   //delay(750);
   //servo.write(90);
   delay(500);
   Serial.print("Distancia direita eh: ");
   Serial.println (cmd);
   delay(300);
   return cmd;
}   

//---------------------------------------CONDICIONAL--------------------------------------------------

void casos(){                      //Decide o que será feito quando o carrinho achar um obstaculo
        
	if(TESTE == 1){                //Vira a direita e avalia se pode andar
		parar();
                virarDireita();
		avaliarDistanciaFrontal();
	}

	if(TESTE == 2){            //Vira a esquerda e avalia se pode andar
                parar();
		virarEsquerda();
                //virarEsquerda();
		avaliarDistanciaFrontal();
	}

	if(TESTE != 2 && TESTE != 1){                     //Da ré para depois virar e avaliar denovo
		virarDireita();
		avaliarDistanciaTraseira();
	}
}

//--------------------------------CONDICIONAL TESTE!!!!! -----------------------------

void casosTeste(){                      //Decide o que será feito quando o carrinho achar um obstaculo
        int distanciaEsquerda = verificaEsquerdaServo(); 
        int distanciaDireita = verificaDireitaServo();     
        servo.write(90);            // No final do procedimento o servo estara para direita, por isso a necessitade desse comando
        //int distanciaSuperiorCentro = leSensorFrontalSuperior();       
        if (distanciaEsquerda <= DISTANCIA_COLISAO && distanciaDireita >= DISTANCIA_COLISAO){ // Caso nao tenha espaco para virar a esquerda e sim para direita
          parar();
          delay(500);
          virarDireita();
          parar();
          avaliarDistanciaFrontal();
          delay(1000);
        }
        if (distanciaEsquerda <= DISTANCIA_COLISAO && distanciaDireita <= DISTANCIA_COLISAO){ // caso nao tenha espaco para nenhum lado ele vai dar a re para assim virar
          parar();
          delay(500);
          int sensorTraseiro = leSensorTraseiro();
          if(sensorTraseiro >= DISTANCIA_COLISAO){
            re();
            virarDireita();
            parar();
            avaliarDistanciaFrontal();
            delay(1000);
          }
          else{
            parar();
            avaliarDistanciaFrontal();
          }
        }  
        if (distanciaDireita <= DISTANCIA_COLISAO && distanciaEsquerda >= DISTANCIA_COLISAO){ //caso tenha nao tenha espaco para virar pra direta e sim para esquerda
          parar();
          delay(500);
          virarEsquerda();
          avaliarDistanciaFrontal();
          delay(1000);
        }
}        


