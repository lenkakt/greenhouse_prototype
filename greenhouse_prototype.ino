// Greenhouse demo application for Seeeduino XIAO used for demonstrations 
// Author: Lenka Koskova Triskova, lenka@koskova.cz
// License: MIT


// Greenhouse main internal states
#define GH_STATE_SLEEP 1              //Device in sleep mode to save energy
#define GH_STATE_ON 2                 //Device in normal operation 
#define GH_STATE_UI 3                 //Device interacting with user
#define GH_STATE_INIT 4               //Device is initializing
#define GH_STATE_CLOSING 5            //Device is switching off, closing connections etc. 
#define GH_STATE_ERROR 6              //Device faces some error; restart needed

#define GH_ACT_IN_TEMP 1
#define GH_ACT_IN_HUMIDITY 2
#define GH_ACT_OUT_WATER 3
#define GH_ACT_OUT_DOOR 4
#define GH_ACT_TELEMETRY 5
#define GH_ACT_UI_LOCAL 6
#define GH_ACT_UI_BT 7
#define GH_ACT_UI_NET 8
#define GH_ACT_CLOUD 9

#define GH_DELAY_SHORT 1000
#define GH_DELAY_LONG 3000

#define GH_ERR_NOERR 0 //No error detected, normal operation
#define GH_ERR_NOINIT 1 //Device starting init in wrong state
#define GH_ERR_UNKNOWN_STATE 2 //Device in undefined state (restart needed).


#define GH_DEBUG_OFF 0 //No debug prints
#define GH_DEBUG_ON 1 // Debug levels
#define GH_DEBUG_STRING "GH DEBUG: "


int gh_state=GH_STATE_INIT;
int gh_err_code=GH_ERR_NOERR;
int gh_debug=GH_DEBUG_ON;

int gh_loop_count = 0;
int gh_restart = 0;


int gh_process_error(){
  int ret_code = 0;
  //Returns 1 if restart is needed.
  switch(gh_err_code) {
    case GH_ERR_NOINIT:
      Serial.printf("Device in wrong state at init, restarting.\n");
      gh_state=GH_STATE_CLOSING;
      ret_code = 1;
      break;
    case GH_ERR_UNKNOWN_STATE: 
      Serial.printf("Device in undefined state, restarting.\n");
      gh_state=GH_STATE_CLOSING;
      ret_code=1;
      break;
    default:
      Serial.printf("Unsupported err code, back to normal operation.\n");
      gh_state=GH_STATE_ON;
      ret_code = 0;
      break;
  }
  return ret_code;
} 
void print_debug(String to_print) {
  if (gh_debug == GH_DEBUG_ON) {
    // Use %s as a placeholder for a string
    String full_print = GH_DEBUG_STRING + to_print;
    Serial.print(full_print);
  }
}
void setup() {
  //Serial line initialization:
  if (gh_state == GH_STATE_INIT) {
  Serial.begin(9600);
  Serial.printf("Starting...;\n");
  Serial.printf("Initialization done.\n");
  delay(GH_DELAY_LONG);
  gh_state = GH_STATE_ON;
  }
    else {
      gh_state = GH_STATE_ERROR;
      gh_err_code = GH_ERR_NOINIT;      
  }
  
}


void loop() {

  while (true) { //Main while loop:
    Serial.printf("GH main loop nr: %d\n", gh_loop_count);
    gh_loop_count++;
    // The main loop code:
    // Case state of the device:
    switch(gh_state) {
      case GH_STATE_ON:
        print_debug("State: Normal operation\n");
        break;

      case(GH_STATE_SLEEP):
        print_debug("State: Sleeping\n");
        break;

      case (GH_STATE_UI):
        print_debug("State: User interaction\n");
        break;

      case (GH_STATE_CLOSING):
        print_debug("State: The device is swiching off.\n");
        // Code to close what is open here:

        delay(GH_DELAY_SHORT);
        NVIC_SystemReset();        
        break;

      case (GH_STATE_ERROR):
        print_debug("State: The device is in error state.\n");
        //Process the error and if returns 1, restart the device:
        if (gh_process_error()) {
          NVIC_SystemReset();
        }
        break;

      default:
        print_debug("Unknown state - entering error\n");
        gh_state = GH_STATE_ERROR;
        gh_err_code = GH_ERR_UNKNOWN_STATE;
        break;      
      
    }
    delay(GH_DELAY_LONG);
  }

}
