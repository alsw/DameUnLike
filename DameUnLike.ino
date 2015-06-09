#include <Servo.h>
#include <Process.h>
#include <FileIO.h>
#include <LiquidCrystal.h>

#define PinServo 9
#define IdFoto ""
#define TokenFb ""
#define FbFanpage "alswblog"

Servo Servo360;
LiquidCrystal LCD(12, 11, 5, 4, 3, 2);
int Likes  = 0;
int LikesP = 0;
int Pos = 73;

void setup() {
  Servo360.attach(PinServo);
  Servo360.write(Pos);
  LCD.begin(16, 2);
  LCD.clear();
  LCD.print("Liked ALSW:");
  Bridge.begin();
  Serial.begin(9600);
  FileSystem.begin();

  Serial.println("Creardo archivo");
  CrearScript();
  Likes = CorrerScript();
  LikesP = Likes;
  LCD.setCursor(0, 1);
  LCD.print(Likes);
}

void loop() {
  delay(500);
  Likes = CorrerScript();
  if ( Likes !=  LikesP) {
    LikesP =  Likes;
    Servo360.write(0);
    delay(5000);
    Servo360.write(Pos);
    LCD.setCursor(0, 1);
    LCD.print(Likes);
  }
}

void CrearScript() {
  File Script = FileSystem.open("/tmp/LikesNuevos.py", FILE_WRITE);

  Script.print("#!/usr/bin/python\n");
  Script.print("import urllib2,json \n");
  Script.print("data = urllib2.urlopen('");
  Script.print("http://graph.facebook.com/");
  Script.print(FbFanpage);
  Script.print("').read() \n");
  Script.print("json_data = json.loads(data)\n");
  Script.print("print ('%s') % (json_data['likes'])\n");
  Script.close();

  Process chmod;
  chmod.begin("chmod");      // chmod: change mode
  chmod.addParameter("+x");  // x stays for executable
  chmod.addParameter("/tmp/LikesNuevos.py");  // path to the file to make it executable
  chmod.run();
}

int CorrerScript() {
  Process Miscript;
  Miscript.begin("/tmp/LikesNuevos.py");
  Miscript.run();

  String output = "";
  while (Miscript.available()) {
    output += (char)Miscript.read();
  }
  output.trim();
  return output.toInt() ;
}
