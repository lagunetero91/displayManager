int ports [] = {2,3,4,5,6,7,8,9};   // Array con puertos usados.
int my_display [10] [7] = {         // Matriz con diferentes combianciones para formar los números en el display.
    {1,1,1,1,1,1,0},  // 0
    {0,1,1,0,0,0,0},  // 1
    {1,1,0,1,1,0,1},  // 2
    {1,1,1,1,0,0,1},  // 3
    {0,1,1,0,0,1,1},  // 4
    {1,0,1,1,0,1,1},  // 5
    {1,0,1,1,1,1,1},  // 6
    {1,1,1,0,0,0,0},  // 7
    {1,1,1,1,1,1,1},  // 8
    {1,1,1,0,0,1,1},  // 9
  };
  
int level;          // Variable usada para hacer oscilar los valores del pin 13.
int count_speed;    // Variable que da el tiempo del delay.
int port_osci = 13; // Puerto usado para medir con el osciloscopio.   
String new_command; // Varaible donde se alamcena el comando introducido por puerto serie.
String command;     // Variable que alamacena el comando si es corecto.



void setup(){
  count_speed = 1000;  // Tiempo inicial del delay 1 sec.
  Serial.begin(9600);  // Inicializar el puerto sere
  command= "pause off";
  pinMode(port_osci, OUTPUT);  // Inicializar puerto 13.
  digitalWrite(port_osci, LOW);
  level = 0;
  for (int i = 0; i < 8; i++){
    pinMode(ports[i], OUTPUT);      // Inicializar los puertos de salida a nivel bajo.
    digitalWrite(ports[i],LOW);
  }  
}



void loop(){
  
  increment();  //Función que hace la cuenta incremental.
  decrement();  //Función que hace la cuenta decremental.
 
}



  // Función que chequea que los comandos sean validos.
  boolean valid_command(String str){
    String str_aux = str;
    if( str == "reset" || str == "pause on" || str == "pause off" || 
       (str.length() == 8 && str_aux.substring(0,5) == "go to" && (str.charAt(6) == 43 || str.charAt(6) == 45) && str.charAt(7) > 47 && str.charAt(7) < 58) || 
       (str.length() ==  7 && str_aux.substring(0,5) == "speed" && (str.charAt(6) == 43 || str.charAt(6)==45 || str.charAt(6)==35))){
         
      Serial.print("Se ha ejecutado el comando \""); Serial.print(str); Serial.println("\".");
      return true;
    } else {
      Serial.print("El comando \""); Serial.print(str);Serial.print("\"");Serial.println(" no es valido.");
      return false;
    } 
  }



  void increment (){
   
    for(int i = 0; i<10; i++ ){
      change_port_osci();
      for(int j = 0; j<7; j++ ){
        if (Serial.available()>0){
          new_command=Serial.readString();
          new_command.toLowerCase();
          if(valid_command(new_command)){
            command = new_command; 
          }
        }
        
        if (command == "reset"){    // Si el comando es reset inicializamos los contadores del bucle.
          i=0;
          j=0;
          command = "pause off";
        }
        
        if (command.substring(0,5) == "go to" && command.charAt(6) == 43){     
           i= command.charAt(7)-48;
           j=0;
           command = "pause off";  
        }else if (command.substring(0,5) == "go to" && command.charAt(6) == 45){
          return;
        }
        
        if (command.substring(0,5) == "speed"){     
           modify_speed(command.charAt(6)); 
            command = "pause off";
        }
    
        if(command == "pause off"){  // Se comprueba si se debe paralizar la cuenta.
          digitalWrite(9,LOW);
          digitalWrite(ports[j],my_display[i][j]);
        }
      }
      delay(count_speed);
      
    }
  }




  void decrement (){
       
    for(int i = 9; i>=0; i-- ){
      change_port_osci();
      for(int j = 6; j>=0; j-- ){
        if (Serial.available()>0){
          new_command=Serial.readString();
          new_command.toLowerCase();
          if(valid_command(new_command)){
            command = new_command; 
          }
        }
        if (command == "reset"){    // Si el comando es reset, salimos de la función de decrementar.
          return;
        }
        
        if (command.substring(0,5) == "go to" && command.charAt(6)==45){  // Si el comando es go to x, salimos de decrementar.
           i= command.charAt(7)-48;
           j=6;
           command = "pause off"; 
        } else if (command.substring(0,5) == "go to" && command.charAt(6)==43) {
           return;  
        }
        
         if (command.substring(0,5) == "speed"){    // Si el comando es speed llamamos a la función modify_speed. 
           modify_speed(command.charAt(6));
           command = "pause off";
        }
        
        if(command == "pause off"){  // Se comprueba si se debe paralizar la cuenta.
          digitalWrite(9,HIGH); 
          digitalWrite(ports[j],my_display[i][j]);
        }
      }
    
    delay(count_speed);
    }  
  }
  
  
  
  
  void change_port_osci(){  // Función encargadad de cambiar el valor de la señal del pin 13
    if ( level == 1 ){
      digitalWrite(port_osci,HIGH);
      level = 0;
    } else {
    digitalWrite(port_osci,LOW);  
    level = 1;
    }
  }





  void modify_speed(int x){  //  Funcíon encargada de cambiar el tiempo de los delays.
     if (x == 43 ){
       if(count_speed > 200){
          count_speed = count_speed - 100;
       }
     } else if(x == 45){
       count_speed = count_speed + 100;
     } else {
       count_speed = 1000; 
     }
  }
