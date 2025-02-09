#define BUZZER_PIN 21
#define BUTTON_PIN 34
hw_timer_t *timer = NULL;
volatile int noteIndex = 0;
volatile int speedIndex = 0;
bool state = false;
// โน้ตเพลง Twinkle Twinkle Little Star
const int notes[] = { 1047, 1109, 1175,1245,1319,1397,1480,1568,1661,1760,1865,1976};
//ระยะห่างโน๊ต               
const int noteDurations[] = {500, 500, 500, 500, 500, 500, 1000,
                   500, 500, 500, 500, 500, 500, 1000};
//ปรับความเร็ว
const float speedLevels[] = {4.0, 2.0, 1.0, 0.5, 0.25};
unsigned long lastPressTime = 0;

void IRAM_ATTR toggleBuzzer() {
    state = !state;
    digitalWrite(BUZZER_PIN, state);
}
void IRAM_ATTR changeSpeed() {
    unsigned long currentTime = millis();
    if (currentTime - lastPressTime > 1000) { 
        speedIndex = (speedIndex + 1) % 5;
        lastPressTime = currentTime; 
    }
}
void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), changeSpeed, FALLING);
    // ตั้งค่าTimer
    timer = timerBegin(1000000); 
    timerAttachInterrupt(timer, &toggleBuzzer);
    timerAlarm(timer, (1000000 / (2 * notes[noteIndex])), true , 0);
}
void loop() {
    static unsigned long lastNoteChange = 0;
    unsigned long noteDuration = noteDurations[noteIndex] * speedLevels[speedIndex];
    if (millis() - lastNoteChange >= noteDuration) {
        lastNoteChange = millis();
        noteIndex++;
        if (noteIndex >= sizeof(notes) / sizeof(notes[0])) {
            noteIndex = 0;
        }
        timerAlarm(timer, (1000000 / (2 * notes[noteIndex])), true , 0);
    }
}