
#define RS485Transmit    HIGH
#define RS485Receive     LOW 
#define SLAVE   1
#define LOOPTIME 100

void setup() {
  // put your setup code here, to run once:

  pinMode(8, OUTPUT);
  digitalWrite(8, RS485Receive); //DE,RE=LOW, RX enabled

  Serial.begin(115200);     // Console 
  Serial1.begin (1000000);  // TO MAX485
}

int recv;
int i;
unsigned long lastMilli = 0;                    // loop timing 
long dT = 0;

void loop() {
  // put your main code here, to run repeatedly:
  
  if((millis()-lastMilli) >= LOOPTIME)  {                                    // enter tmed loop
    dT = millis()-lastMilli;
    lastMilli = millis();
    
    Serial.print("Sent: ");
    Serial.println(i, HEX);
    
    sendCmd(i, SLAVE);
    recv = read_slave(SLAVE);
      

    Serial.print("Got: ");
    Serial.println(recv, HEX);

    i++;
    if (i>100) i = 0;
    
  }


  /*
  while(1)  {
    //i = read_slave(SLAVE);
    if (Serial1.available() > 0) {
      char c = (char) Serial1.read();
      Serial.println(c, HEX);
    }
  }
  */

}

int read_slave(int slave)
{
 
  if (Serial1.available() >= 4) 
  {
    char rT = (char)Serial1.read(); //read target speed from mega
          if(rT == '{') //start byte
          {
              char commandArray[3];
              Serial1.readBytes(commandArray,3);
              byte rA=commandArray[0]; //device address
              byte rH=commandArray[1]; //high byte
              char rP=commandArray[2]; //stop byte

            
              
              if(rP=='}' && rA==slave)         
              {                
                  return rH;
              } 
          }
  } 
    
  
}

void sendCmd(int cmd,int slave)
{
  
  digitalWrite(8, RS485Transmit); //DE,RE=HIGH, TX enabled

  char sT='{';  //send start byte
  byte sA = slave;
  byte sH = lowByte(cmd);
  char sP='}';  //send stop byte
  Serial1.write(sT); 
  Serial1.write(sA); 
  Serial1.write(sH); 
  Serial1.write(sP);
  delayMicroseconds(10);
  digitalWrite(8, RS485Receive); //DE,RE=LOW, RX enabled
  delayMicroseconds(100);  
}

