#include "pebble.h"


static Window *window;
TextLayer *header;

// This is a menu layer
// You have more control than with a simple menu layer

static int counter = 0;

static char *title;

// Each section has a number of items;  we use a callback to specify this
// You can also dynamically add and remove items using this





// This initializes the menu upon window load
void window_load(Window *window) {

  // Now we prepare to initialize the menu layer
  // We need the bounds to specify the menu layer's viewport size
  // In this case, it'll be the same as the window's
  //Layer *window_layer = window_get_root_layer(window);
  //GRect bounds = {0, 0, };

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
      switch(counter){

        case(0):

        text_tuple = dict_find(iter, 0);
        title = malloc(text_tuple->length);
        strncpy(title, text_tuple->value->cstring, text_tuple->length);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Title: %s", title);
        break;
        case(1):

        APP_LOG(APP_LOG_LEVEL_DEBUG, "Not active rn");

        break;
        case(2):

        APP_LOG(APP_LOG_LEVEL_DEBUG, "Not active rn");

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
