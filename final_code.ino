#include <WiFi.h>
#include <WebServer.h>
#include <Deneyap_Servo.h>

const char* ssid = "iPhone de Gustavo";
const char* password = "Sgfidelis";
int i = 0;
int ENA = 14;
int ENB = 12;
int IN1 = 13;
int IN2 = 16;
int IN3 = 17;
int IN4 = 18;

WebServer server(80);
Servo myservo;

void setup() {
  Serial.begin(115200);
  myservo.attach(15);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  myservo.write(60);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/leitura", HTTP_GET, handleleitura);
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  analogWrite(IN1, 800);
  analogWrite(IN2, 0);
  analogWrite(ENA, 0);
  analogWrite(IN3, 800);
  analogWrite(IN4, 0);
  analogWrite(ENB, 0);
}

void loop() {
 
   server.handleClient();
   delay(10);
}

void handleRoot() {
  server.send(200, "text/html", "<!DOCTYPE html>\
<html>\
<head>\
<title>ESP32</title>\
<style>\
.prevent-select{-webkit-user-select:none;-ms-user-select:none;user-select:none;}\
@font-face{font-family:'Droid_Sans_Bold';src:url('Fonts/Droid_Sans_Bold');}\
h1{font-size:50px;}h2{font-size:30px;}.column_1{background-color:rgba(119,91,241,0.226);}\
</style>\
</head>\
<body>\
<div class='column_1'>\
<h1 style='text-align:center;' class='prevent-select'>Project boat</h1>\
<h2 style='text-align:center;' class='prevent-select'>\
X: <span id='x_coordinate' name='Xpos'></span> \
Y: <span id='y_coordinate' name='Ypos'></span> \
Speed: <span id='speed' name='speed'></span>% \
Angle: <span id='angle' name='angle'></span>\
Waterjet: <span id='Waterjet' name='Waterjet'></span>\
</h2>\
</div>\
<canvas id='canvas' name='game'></canvas>\
<script>\
var canvas,ctx;\
window.addEventListener('load',()=>{\
  canvas=document.getElementById('canvas');\
  ctx=canvas.getContext('2d');\
  resize();\
  document.addEventListener('mousedown',startDrawing);\
  document.addEventListener('mouseup',stopDrawing);\
  document.addEventListener('mousemove',Draw);\
  document.addEventListener('touchstart',startDrawing);\
  document.addEventListener('touchend',stopDrawing);\
  document.addEventListener('touchcancel',stopDrawing);\
  document.addEventListener('touchmove',Draw);\
  window.addEventListener('resize',resize);\
  document.getElementById('x_coordinate').innerText=0;\
  document.getElementById('y_coordinate').innerText=0;\
  document.getElementById('speed').innerText=0;\
  document.getElementById('angle').innerText=0;\
});\
var width,height,radius,x_orig,y_orig;\
function SEND(x, y, speed, angle, waterjet) {\
   var xhttp = new XMLHttpRequest();\
   xhttp.open('GET', '/leitura?coordinateX=' + x  + '&coordinateY=' + y + '&speed=' + speed + '&angle=' + angle + '&Waterjet=' + waterjet, true);xhttp.send()}\
function resize(){\
  width=window.innerWidth;\
  radius=200;\
  height=radius*6.5;\
  ctx.canvas.width=width;\
  ctx.canvas.height=height;\
  background();\
  joystick(width/3,height/3);\
  attack('#0000FF');\
  bar((coord.y+200)/4);\
  speedbar(0);\
}\
function background(){\
  document.body.style.backgroundColor='yellow';\
  x_orig=width/3;\
  y_orig=height/3;\
  x_attack=width*2/3;\
  y_attack=height*22/30;\
  ctx.beginPath();\
  ctx.fillStyle='#CCCC00';\
  ctx.fillRect(0,0,width,height);\
  ctx.arc(x_orig,y_orig,radius+20,0,Math.PI*2,true);\
  ctx.fillStyle='#ECE5E5';\
  ctx.fill();\
}\
function joystick(width,height){\
  ctx.beginPath();\
  ctx.arc(width,height,radius,0,Math.PI*2,true);\
  ctx.fillStyle='blue';\
  ctx.fill();\
  ctx.strokeStyle='darkblue';\
  ctx.lineWidth=8;\
  ctx.stroke();\
  ctx.fillStyle='#000000';\
  ctx.fillRect(width-10,height-50 ,20,-150);\
  ctx.fillRect(width-50,height-10 ,-150,20);\
  ctx.fillRect(width+50,height+10 ,150,-20);\
  ctx.fillRect(width+10,height+50 ,-20,150);\
}\
function attack(color){\
  ctx.beginPath();\
  ctx.arc(width*2/3,height*22/30,radius,0,Math.PI*2,true);\
  ctx.fillStyle=color;\
  ctx.fill();\
  ctx.font='100px Impact';\
  ctx.fillStyle='#FFFFFF';\
  ctx.fillText('ATTACK',width*2/3-145,height*22/30+35);\
}\
function bar(percentage){\
  ctx.beginPath();\
  ctx.arc(width*9/10,height*22/30,10,0,Math.PI*2,true);\
  ctx.fillStyle='blue';\
  ctx.fill();\
  ctx.fillStyle='#FFFFFF';\
  ctx.fillRect(width*9/10-10,height*22/30,20,-500);\
  ctx.fillStyle='blue';\
  if (percentage > 100) {\
    percentage = 100;\
  }\
  ctx.fillRect(width*9/10-10,height*22/30,20,percentage*(-5));\
  ctx.font='10px Verdana';\
  ctx.fillStyle='blue';\
  ctx.font = '80px Impact'\
  ctx.fillText('BATTERY',width*9/10-100,height*22/30+100);\
  ctx.fillText(percentage+'%',width*9/10+30,height*22/30-200);\
}\
function speedbar(percentage){\
  ctx.beginPath();\
  ctx.arc(width*1/10,height*22/30,10,0,Math.PI*2,true);\
  ctx.fillStyle='blue';\
  ctx.fill();\
  ctx.fillStyle='#FFFFFF';\
  ctx.fillRect(width*1/10-10,height*22/30,20,-500);\
  ctx.fillStyle='blue';\
  percentage = Math.round(percentage/2);\
  if (percentage < 0) {\
    percentage = percentage*(-1);\
  }\
  if (percentage > 100) {\
    percentage = 100;\
  }\
  ctx.fillRect(width*1/10-10,height*22/30,20,percentage*(-5));\
  ctx.font='10px Verdana';\
  ctx.fillStyle='blue';\
  ctx.font = '80px Impact'\
  ctx.fillText('SPEED',width*1/10-100,height*22/30+100);\
  ctx.fillText(percentage+'%',width*1/10+30,height*22/30-200);\
}\
let coord={x:0,y:0};\
let paint=false;\
function getPosition(event){\
  var mouse_x=event.clientX||event.touches[0].clientX;\
  var mouse_y=event.clientY||event.touches[0].clientY;\
  coord.x=mouse_x-canvas.offsetLeft;\
  coord.y=mouse_y-canvas.offsetTop;\
}\
function is_it_in_the_circle(x,y){\
  var current_radius=Math.sqrt(Math.pow(coord.x-x,2)+Math.pow(coord.y-y,2));\
  if(radius>=current_radius)return true;\
  else return false;\
}\
function startDrawing(event){\
  paint=true;\
  getPosition(event);\
  if(is_it_in_the_circle(x_orig,y_orig)){\
    ctx.clearRect(0,0,canvas.width,canvas.height);\
    background();\
    attack('#2727d6');\
    joystick(coord.x,coord.y);\
    bar((coord.y+200)/4);\
    speedbar(coord.y-height/3);\
    flag=true;\
    Draw();\
  }else{\
    flag=false;\
    if(is_it_in_the_circle(x_attack,y_attack)){\
      attack('#add8e6');\
      SEND(0,0,0,0,1);\
      document.getElementById('Waterjet').innerText=1;\
    }else{\
      attack('#0000FF');\
    }\
  }\
}\
function stopDrawing(){\
  paint=false;\
  ctx.clearRect(0,0,canvas.width,canvas.height);\
  background();\
  attack('#0000FF');\
  bar((coord.y+200)/4);\
  speedbar(0);\
  joystick(width/3,height/3);\
  document.getElementById('x_coordinate').innerText=0;\
  document.getElementById('y_coordinate').innerText=0;\
  document.getElementById('speed').innerText=0;\
  document.getElementById('angle').innerText=0;\
  document.getElementById('Waterjet').innerText=0;\
  SEND(0,0,0,0,0);\
}\
function Draw(event){\
  if(paint){\
    ctx.clearRect(0,0,canvas.width,canvas.height);\
    background();\
    attack('#0000FF');\
    bar((coord.y+200)/4);\
    \
    var angle_in_degrees,x,y,speed;\
    var angle=Math.atan2((coord.y-y_orig),(coord.x-x_orig));\
    if(Math.sign(angle)==-1){\
      angle_in_degrees=Math.round(-angle*180/Math.PI);\
    }else{\
      angle_in_degrees=Math.round(360-angle*180/Math.PI);\
    }\
  if(is_it_in_the_circle(x_orig,y_orig)){\
    if(flag){\
      joystick(coord.x,coord.y);\
      speedbar(coord.y-height/3);\
      x=coord.x;\
      y=coord.y;\
      getPosition(event);\
      speed=Math.round(Math.abs((y-y_orig)/2));\
      var x_relative=Math.round(x-x_orig);\
      var y_relative=Math.round(y-y_orig);\
      document.getElementById('x_coordinate').innerText=x_relative;\
      document.getElementById('y_coordinate').innerText=y_relative;\
      document.getElementById('speed').innerText=speed;\
      document.getElementById('angle').innerText=angle_in_degrees;\
      document.getElementById('Waterjet').innerText=0;\
      SEND(x_relative,y_relative,speed,angle_in_degrees, 0);\
  }else{\
      joystick(width/3,height/3);\
      speedbar(0);\
      SEND(0,0,0,0, 0);\
  }\
  }else{\
    x=radius*Math.cos(angle)+x_orig;\
    y=radius*Math.sin(angle)+y_orig;\
    if(flag){\
      joystick(x,y);\
      speedbar(coord.y-height/3);\
      getPosition(event);\
      speed=Math.round(Math.abs((y-y_orig)/2));\
      var x_relative=Math.round(x-x_orig);\
      var y_relative=Math.round(y-y_orig);\
      document.getElementById('x_coordinate').innerText=x_relative;\
      document.getElementById('y_coordinate').innerText=y_relative;\
      document.getElementById('speed').innerText=speed;\
      document.getElementById('angle').innerText=angle_in_degrees;\
      document.getElementById('Waterjet').innerText=0;\
      SEND(x_relative,y_relative,speed,angle_in_degrees, 0);\
    }else{\
      joystick(width/3,height/3);\
      speedbar(0);\
      SEND(0,0,0,0, 0);\
    }\
  }\
}\
}\
</script>\
</body>\
</html>");
}
void handleleitura() {
  if (server.hasArg("coordinateX") && server.hasArg("coordinateY") && server.hasArg("speed") && server.hasArg("angle") && server.hasArg("Waterjet")) {
  
    String X = server.arg("coordinateX");
    int coX = server.arg("coordinateX").toInt();
    String Y = server.arg("coordinateY");
    int coY = server.arg("coordinateY").toInt();
    String speedV = server.arg("speed");
    String angle = server.arg("angle");
    int jet = server.arg("Waterjet").toInt();
    
    Serial.print("X coordinate = "); 
    Serial.print(X); 
    Serial.print(" Y coordinate = "); 
    Serial.print(Y); 
    Serial.print(" speed = "); 
    Serial.print(speedV); 
    Serial.print(" angle = "); 
    Serial.println(angle);
    
    coX = map(coX, -200, 200, 0, 200);
    coY = map(coY, -200, 200, 1024, -1024);
    if (coY < 0) {
    analogWrite(IN1, 800);
    analogWrite(IN2, 0);
    analogWrite(ENA, coY*(-1));
    } else {
    analogWrite(IN1, 0);
    analogWrite(IN2, 800);
    analogWrite(ENA, coY);
    }
    if (jet == 1) {
      analogWrite(ENB, 1023);
    } else {
      analogWrite(ENB, 0);
    }
    
    
    myservo.write(coX);  
    server.send(200, "text/plain", "LED state changed");
}
  
  }
