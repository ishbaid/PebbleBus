#include "pebble.h"


static Window *window;
TextLayer *header;

// This is a menu layer
// You have more control than with a simple menu layer

static int counter = 0;
static int *arrivalTimes;
static int *buses;

static char *title;

char *answer[]={"Commuter Southbound",
 "Commuter Northbound",
 "Northwood",
"Bursley-Baits",
"Bursley-Baits (Nights)",
"Intercampus to East Campus"
"Intercampus to NIB",
"Mitchell-Glazier to Glazier and VA",
"KMS Shuttle",
"Oxford Shuttle",
"Diag to Diag express"
"Commuter Northbound (Nights)"
"Oxford Loop to Diag to Diag Express",
"North Campus",
"Night Owl",
"Bursley-Baits",
"Northwood",
"Oxford Shuttle"};



// This initializes the menu upon window load
void window_load(Window *window) {


  header = text_layer_create(GRect(0, 0 , 144, 60));

  // Add it to the window for display
  title = malloc(14);
  title = "Bus Stop Name";

}

void window_unload(Window *window) {
  // Destroy the menu layer
  text_layer_destroy(header);

}

////////////////////////Handles messages
 void out_sent_handler(DictionaryIterator *sent, void *context) {
   // outgoing message was delivered
 }


 void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
   // outgoing message failed
 }

 void in_received_handler(DictionaryIterator *iter, void *context) {
   // incoming message received
  // Check for fields you expect to receive
  /*int KEY = 1; 
  Tuple *text_tuple = dict_find(iter, KEY);

  // Act on the found fields received
  if (text_tuple) {
    
      
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Text: %s", text_tuple->value->cstring);
      // if(title){

      //   //free(title);
      //   title = NULL;
      // }
      title = malloc(text_tuple->length);
      strncpy(title, text_tuple->value->cstring, text_tuple->length);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Title: %s", title);
      text_layer_set_text(header, title);
      layer_add_child(window_get_root_layer(window), text_layer_get_layer(header));

  }*/

      Tuple *text_tuple; 
      Tuple *tuple;
      Tuple *tuple2;
      Tuple *tuple3;
      int size = 0;
      int i = 0;
      switch(counter){

        case(0):
          text_tuple = dict_find(iter, 0);
            if(text_tuple){

              title = malloc(text_tuple->length);
              strncpy(title, text_tuple->value->cstring, text_tuple->length);
              text_layer_set_text(header, title);
              layer_add_child(window_get_root_layer(window), text_layer_get_layer(header));
              APP_LOG(APP_LOG_LEVEL_DEBUG, "Title: %s", title);
            }

        break;
        case(1):

            tuple = dict_find(iter, 0);
            if(tuple){

              size = tuple->value->int32;
              APP_LOG(APP_LOG_LEVEL_DEBUG, "Route Size is %i", size);
            }
            

        break;
        case(2):

            while (tuple2) {

               *arrivalTimes ++ = tuple2->value->int32;

               tuple2 = dict_read_next(iter);
               i ++;
            }

            for(i = 0; i < 5; i ++){

                APP_LOG(APP_LOG_LEVEL_DEBUG, "%i is %i", i, arrivalTimes[i]);

            }

        break;
        case(3):

            tuple3 = dict_find(iter, 0);
            while (tuple3 && i < 5) {

               *buses ++ = tuple3->value->int32;
               tuple3 = dict_read_next(iter);
               i ++;
            }

            for(i = 0; i < 5; i ++){

                APP_LOG(APP_LOG_LEVEL_DEBUG, "%i is %i", i, buses[i]);

            }
        break;
        default:

            APP_LOG(APP_LOG_LEVEL_DEBUG, "Default");
        break;

      };

      counter ++;

 }


 void in_dropped_handler(AppMessageResult reason, void *context) {
   // incoming message dropped
  APP_LOG(APP_LOG_LEVEL_DEBUG, "REASON: %i", reason);
 }

int main(void) {
  

  arrivalTimes = (int *)malloc(sizeof(int)*5);
  buses = (int *)malloc(sizeof(int)*5);

  //set up messages
   app_message_register_inbox_received(in_received_handler);
   app_message_register_inbox_dropped(in_dropped_handler);
   app_message_register_outbox_sent(out_sent_handler);
   app_message_register_outbox_failed(out_failed_handler);
  
  //
   const uint32_t inbound_size = 64;
   const uint32_t outbound_size = 64;
   app_message_open(inbound_size, outbound_size);
  
  
  window = window_create();

  // Setup the window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true /* Animated */);

  app_event_loop();

  window_destroy(window);
}
