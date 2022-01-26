
//PIR Stuff and sound stuff
int pirPin = 3; // Input for HC-S501
int pirValue; // Place to store read PIR Value 
#define speakerPin 5
void setup() {}

//Phrases Generator
void phrase1() {
    int k = random(500,4000);
    for (int i = 0; i <=  random(50,2000); i++){
        tone(speakerPin, k+(-i*2));          
        delay(random(.9,2));             
    } 
    for (int i = 0; i <= random(50,1000); i++){
        tone(speakerPin, k + (i * 10));          
        delay(random(.5,4));             
    } }
void phrase2() {
    int k = random(500,4000);
    for (int i = 0; i <= random(50,2000); i++){
        tone(speakerPin, k+(i*2));          
        delay(random(.9,2));             
    } 
    for (int i = 0; i <= random(50,1000); i++){
        tone(speakerPin, k + (-i * 10));          
        delay(random(.5,4));             
    } }

void phrase3() {
    int k = random(200,8000);
    for (int i = 0; i <= random(10,500); i++){
        tone(speakerPin, k+(i*2));          
        delay(random(.20,5));             
    } 
    for (int i = 0; i <= random(50,2000); i++){
        tone(speakerPin, k + (-i * 10));          
        delay(random(.5,4));             
    } }
    
void loop() {
  //PIR Set
pirValue = digitalRead(pirPin);

delay(30);
    if((pirValue == HIGH)){ // PIR reading HIGH (true), initiating tone function
    int K = 2000;
    switch (random(1,9)) {
        case 1:phrase1(); break;
        case 2:phrase2(); break;
        case 3:phrase1(); phrase2(); break;
        case 4:phrase3(); phrase2(); phrase1();break;
        case 5:phrase1(); phrase2(); phrase1(); phrase3(); phrase1();break;
        case 6:phrase2(); phrase1(); phrase2(); break;
        case 7:phrase1(); phrase1(); phrase2(); phrase2(); phrase1(); break;
        case 8:phrase1(); phrase2(); phrase1(); break;
        case 9:phrase3(); phrase1(); phrase3(); phrase2(); break;
    }
    for (int i = 0; i <= random(3, 9); i++){
        tone(speakerPin, K + random(-1700, 2000));          
        delay(random(70, 170));  
        noTone(speakerPin);         
        delay(random(0, 30));             
    } 
    noTone(speakerPin);         
    delay(random(50, 200));
  }
}
