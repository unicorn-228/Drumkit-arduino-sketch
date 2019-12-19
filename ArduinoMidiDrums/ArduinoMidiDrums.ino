#define BassDrumDelay 300
#define HatThresholdColsed 180
#define HatDigitalPin 28


struct drum       // Структура для хранения информации об инструменте
{
  bool IsActive;  // Признак звучания инструмента в данный момент
  byte Note;      // Нота
  bool isHat;
  int Threshold;  // Порог чувствительности
  int PlayTime;   // Максимальное время звучания (мс)
  unsigned long NoteOffTime; // Время снятия ноты
  
};

drum Drums[] = // Массив инструментов (не больше 6). Номера мнструментов можно подсмотреть в Википедии: https://en.wikipedia.org/wiki/Percussion_instrument
{
  {0, 46 , true , 450, 100, 0}, // 42 closed hat
 {0, 38, false , 80, 90, 0}, // snare
 {0, 41, false, 40, 90, 0}, // tom low
 {0, 47, false, 40, 90, 0 }, // tom high
  {0 , 49, false , 100 , 50 ,0}, // crash
  {0 , 35, false , 350 , 90 ,0} // bas drumm
  //{0, 40, 50, 50, 0},
  //{0, 49, 50, 50, 0}
};

int DrumCount = sizeof(Drums) / sizeof(Drums[0]);
int hit = 0;
int i;
bool kostyl = false;
bool firstTimeAfterKostylChange = true;
long long numOfDelay = 0;

int note_tmp = 46;
int threshold_tmp = Drums[0].Threshold;

void setup()
{
  pinMode(A0 , INPUT);
  pinMode(A1 , INPUT);
  pinMode(A2 , INPUT);
  pinMode(A3 , INPUT);
  pinMode(A4 , INPUT);
  pinMode(A5 , INPUT);
  pinMode(7 , INPUT);
  Serial.begin(115200);
  pinMode(28 , INPUT);
  pinMode(13 , OUTPUT);
}

void loop()
{
  if(digitalRead(HatDigitalPin) != 0){
             note_tmp = 42;
             digitalWrite(13 , HIGH);
             threshold_tmp = HatThresholdColsed;
   }
    else {
      note_tmp = 46;
             digitalWrite(13 , LOW);
             threshold_tmp = Drums[0].Threshold;
    }

  for(int i = 0 ; i < DrumCount ; i++) {
    if(i != 8) {
      hit = analogRead(i);
      analogWrite(i , 0);
      if(i != 0) {
        
      if(hit >= Drums[i].Threshold) {
        if(!Drums[i].IsActive) {
          Drums[i].IsActive = true;
          MIDI_TX(144, Drums[i].Note , 127);
          Drums[i].NoteOffTime = millis() + Drums[i].PlayTime;
        }
        else {
          if(millis() >= Drums[i].NoteOffTime) {
            Drums[i].IsActive = false;
            MIDI_TX(128 , Drums[i].Note , 127);
          }
        }
      }
      
      } else {
      
        
        if(hit >= threshold_tmp) {
        if(!Drums[i].IsActive) {
          
          Drums[i].IsActive = true;
          MIDI_TX(144, note_tmp , 127);
          Drums[i].NoteOffTime = millis() + Drums[i].PlayTime;
        }
        else {
          if(millis() >= Drums[i].NoteOffTime) {
            Drums[i].IsActive = false;
            MIDI_TX(128 , note_tmp , 127);
          }
        }
      }
        
      }
    }
  }


  /*
  for (i = 0; i < DrumCount; i++)
  {
    hit = analogRead(i); //  Считываем значение с аналогового входа
    analogWrite(i, 0);
    if ((hit >= Drums[i].Threshold))  // Если значение не ниже порога срабатывания
    {
      if (!Drums[i].IsActive)    // И если нота еще не звучит
      {
        if(millis() - Drums[i].timeSave >= Drums[i].delayTime) {
        // то включаем ноту
        Drums[i].IsActive = true;
        MIDI_TX(144, Drums[i].Note, 127); // Посылаем сообщение взятия ноты (144 = NOTE ON)
        Drums[i].NoteOffTime = millis() + Drums[i].PlayTime;
        Drums[i].timeSave = millis();
        }
      }
      
    }
    else if (Drums[i].IsActive) // Иначе (если нет сигнала с аналогового входа), если нота звучит
    {
      // то проверяем не пора ли ее снять
      if (millis() >= Drums[i].NoteOffTime)
      {
        Drums[i].IsActive = false;
        MIDI_TX(128, Drums[i].Note, 127); // Снимаем (128 = NOTE OFF)
      }
    }
    
  }
  if(digitalRead(7) == HIGH   ){
    if(kostyl == false) {
      if(millis() - numOfDelay >= BassDrumDelay) {
      MIDI_TX(144 , 35 , 127);
      kostyl = true;
      firstTimeAfterKostylChange = true; 
      
      }
      numOfDelay = millis();
    }
   
  } else {
    kostyl = false;
    if(firstTimeAfterKostylChange) {
      MIDI_TX(128 , 35 , 127);
      firstTimeAfterKostylChange = false;
    }
  }

  */
 /* MIDI_TX(144, Drums[0].Note, 127);
  delay(1000);
  MIDI_TX(128, Drums[0].Note, 127);
  delay(3000);*/
}

void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY)
{
  Serial.write(MESSAGE + 9); // 9 - номер MIDI канала для перкуссии
  Serial.write(PITCH);       // Нота
  Serial.write(VELOCITY);    // Громкость
}
