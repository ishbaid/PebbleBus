#include "pebble.h"
#include <stdio.h>
#include <stdlib.h>

//Main Window
static Window *window;

//Layers
ScrollLayer *scroll_layer;
TextLayer *text_layer;
TextLayer *header;



static const int vert_scroll_text_padding = 4;

// This is a menu layer
// You have more control than with a simple menu layer

static int counter = 0;
static int *arrivalTimes;
static int *buses;

static char *title;



char *routes[]={"Commuter Southbound",
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


// scroll_layer for when no vitamins are needed
void scroll_NoneNeeded(){
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  GRect max_text_bounds = GRect(0, 60, bounds.size.w, 2000);

  // Initialize the scroll layer
  scroll_layer = scroll_layer_create(bounds);

  // This binds the scroll layer to the window so that up and down map to scrolling
  // You may use scroll_layer_set_callbacks to add or override interactivity
  scroll_layer_set_click_config_onto_window(scroll_layer, window);

  // Initialize the text layer
  text_layer = text_layer_create(max_text_bounds);
  //A notice


  static char buf[] = "OMG, this is pretty ridiculous";

  snprintf(buf, sizeof(buf), "%d", arrivalTimes[0]);
  
  text_layer_set_text(text_layer, buf);  

  // Change the font to a nice readable one
  // This is system font; you can inspect pebble_fonts.h for all system fonts
  // or you can take a look at feature_custom_font to add your own font
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  // Trim text layer and scroll content to fit text box
  GSize max_size = text_layer_get_content_size(text_layer);
  text_layer_set_size(text_layer, max_size);
  scroll_layer_set_content_size(scroll_layer, GSize(bounds.size.w, max_size.h + vert_scroll_text_padding));

  // Add the layers for display
  scroll_layer_add_child(scroll_layer, text_layer_get_layer(text_layer));

  layer_add_child(window_layer, scroll_layer_get_layer(scroll_layer));
  
}
  


//initializes scroll for vitamins needed
void scroll_init(){
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  GRect max_text_bounds = GRect(0, 0, bounds.size.w, 2000);

  // Initialize the scroll layer
  scroll_layer = scroll_layer_create(bounds);

  // This binds the scroll layer to the window so that up and down map to scrolling
  // You may use scroll_layer_set_callbacks to add or override interactivity
  //THE ORDER MATTERS OF WHICH FUNCTION IS CALLED FIRST
 
  scroll_layer_set_click_config_onto_window(scroll_layer, window);


  // Initialize the text layer
  text_layer = text_layer_create(max_text_bounds);


  // Change the font to a nice readable one
  // This is system font; you can inspect pebble_fonts.h for all system fonts
  // or you can take a look at feature_custom_font to add your own font
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  // Trim text layer and scroll content to fit text box
  GSize max_size = text_layer_get_content_size(text_layer);
  text_layer_set_size(text_layer, max_size);
  scroll_layer_set_content_size(scroll_layer, GSize(bounds.size.w, max_size.h + vert_scroll_text_padding));

  // Add the layers for display
  scroll_layer_add_child(scroll_layer, text_layer_get_layer(text_layer));

  layer_add_child(window_layer, scroll_layer_get_layer(scroll_layer));
  
}


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
  free(arrivalTimes);
  free(buses);
  text_layer_destroy(text_layer);

  scroll_layer_destroy(scroll_layer);

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

            tuple2 = dict_find(iter, 0);
            while (tuple2) {

               arrivalTimes[i] = tuple2->value->int32;

               tuple2 = dict_read_next(iter);
               i ++;
            }

            for(i = 0; i < 5; i ++){

                APP_LOG(APP_LOG_LEVEL_DEBUG, "%i is %i", i, arrivalTimes[i]);

            }

        break;
        case(3):

            tuple3 = dict_find(iter, 0);
            while (tuple3) {

               buses[i] = tuple3->value->int32;
               tuple3 = dict_read_next(iter);
               i ++;
            }

            for(i = 0; i < 5; i ++){

                APP_LOG(APP_LOG_LEVEL_DEBUG, "%i is %i", i, buses[i]);
                if(buses[i] >= 0 && buses[i] < 19){

                  //strcat(allTimes, routes[buses[i]]);
                  
                 // char a[10];
                  //itoa(arrivalTimes[i], a, 10);
                  //char[]a = " ";
                  //strcat(allTimes, a);
                  //char a[20];
                  //sprintf(a, "%d", arrivalTimes[i]);
                  //itoa(arrivalTimes[i], allTimes, 10);
                  //strcat(allTimes, boost::lexical_cast<std::string>(arrivalTimes[i])
                  //strcat(allTimes, "\n");
                }
                

            }

              scroll_NoneNeeded();
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
  text_layer = text_layer_create(GRect(0, 60, 144, 154));
  // Setup the window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true /* Animated */);

  app_event_loop();

  window_destroy(window);
}
