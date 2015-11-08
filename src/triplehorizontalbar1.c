#include <pebble.h>

#define KEY_TOP_BAR_COLOR 0
#define KEY_MIDDLE_BAR_COLOR 1
#define KEY_BOTTOM_BAR_COLOR 2
//#define KEY_TWENTY_FOUR_HOUR_FORMAT 3
#define KEY_BACKGROUND_COLOR 3

static Window *window;
static Layer *s_layer;

static BitmapLayer *s_bt_icon_layer;
static GBitmap *s_bt_icon_bitmap;

static uint8_t s_hour;
static uint8_t s_min;
static uint8_t s_sec;

static int HEIGHT = 25;
static int PADDING = 12;
static int NUM_BARS = 3;

static GBitmap *s_unary_ticks_bitmap;
static BitmapLayer *s_unary_ticks_layer;

static GBitmap *s_ternary_ticks_bitmap;
static BitmapLayer *s_ternary_ticks_layer_minutes;
static BitmapLayer *s_ternary_ticks_layer_seconds;

//static bool twenty_four_hour_format = false;

GColor s_top_bar_color;
GColor s_middle_bar_color;
GColor s_bottom_bar_color;

static void bluetooth_callback(bool connected) {
	// show icon if disconnected
	layer_set_hidden(bitmap_layer_get_layer(s_bt_icon_layer), connected);

	if (!connected) {
		//issue a vibrating alert
		vibes_double_pulse();
	}
}

static void update_time(struct tm * tick_time) {
	s_hour = tick_time->tm_hour;
	s_min = tick_time->tm_min;
	s_sec = tick_time->tm_sec;
	layer_mark_dirty(s_layer);

	//	if (twenty_four_hour_format == true) {
	//		APP_LOG(APP_LOG_LEVEL_DEBUG, "twenty_four_hour_format is true %s", twenty_four_hour_format ? "true" : "false");
	//	}
}

static void set_background_and_text_color(int color) {
	GColor background_color = GColorFromHEX(color);
	//makes text legible over selected background color
	window_set_background_color(window, background_color);
	//text_layer_set_text_color(s_text_layer, gcolor_legible_over(background_color));
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox received handler");
	Tuple *top_bar_color_t = dict_find(iter, KEY_TOP_BAR_COLOR);
	Tuple *middle_bar_color_t = dict_find(iter, KEY_MIDDLE_BAR_COLOR);
	Tuple *bottom_bar_color_t = dict_find(iter, KEY_BOTTOM_BAR_COLOR);
	Tuple *background_color_t = dict_find(iter, KEY_BACKGROUND_COLOR);
	//Tuple *twenty_four_hour_format_t = dict_find(iter, KEY_TWENTY_FOUR_HOUR_FORMAT);

	if (top_bar_color_t) {
		int top_bar_color = top_bar_color_t->value->int32;
		if (top_bar_color == 0) { //quick fix so that black colour persists
			top_bar_color++;
		}

		persist_write_int(KEY_TOP_BAR_COLOR, top_bar_color);

		//set_background_and_text_color(background_color);
		s_top_bar_color = GColorFromHEX(top_bar_color);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "top bar color:  %d", top_bar_color);
	}

	if (middle_bar_color_t) {
		int middle_bar_color = middle_bar_color_t->value->int32;
		if (middle_bar_color == 0) { //quick fix so that black colour persists
			middle_bar_color++;
		}

		persist_write_int(KEY_MIDDLE_BAR_COLOR, middle_bar_color);

		s_middle_bar_color = GColorFromHEX(middle_bar_color);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "middle bar color:  %d", middle_bar_color);
	}

	if (bottom_bar_color_t) {
		int bottom_bar_color = bottom_bar_color_t->value->int32;
		if (bottom_bar_color == 0) { //quick fix so that black colour persists
			bottom_bar_color++;
		}

		persist_write_int(KEY_BOTTOM_BAR_COLOR, bottom_bar_color);

		s_bottom_bar_color = GColorFromHEX(bottom_bar_color);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "bottom bar color:  %d", bottom_bar_color);
	}

	if (background_color_t) {
		int background_color = background_color_t->value->int32;
		if (background_color == 0) { //quick fix so that black colour persists
			background_color++;
		}

		persist_write_int(KEY_BACKGROUND_COLOR, background_color);

		APP_LOG(APP_LOG_LEVEL_DEBUG, "background color: %d", background_color);

		set_background_and_text_color(background_color);
	}

	//if (twenty_four_hour_format) {
	//	twenty_four_hour_format = twenty_four_hour_format_t->value->int8;
	//	persist_write_int(KEY_TWENTY_FOUR_HOUR_FORMAT, twenty_four_hour_format);
	//	//could get struct tm *tick_time and call update_time.. but we don't have a twenty four hour format anyways
	//	//update_time();
	//}
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time(tick_time);
}


static void draw_watchface(Layer *layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(layer);
	uint16_t width = bounds.size.w - (2 * PADDING); //this is 120 on basalt
	uint16_t vert_padding = (bounds.size.h - ((NUM_BARS - 1)*PADDING) - (NUM_BARS*HEIGHT)) / 2;

	//set the colour
	graphics_context_set_fill_color(ctx, COLOR_FALLBACK(s_top_bar_color, GColorRed));

	int8_t cur_hour = s_hour % 12;
	if (cur_hour == 0) {
		cur_hour = 12;
	}

	//display the hour bar
	graphics_fill_rect(ctx, GRect(PADDING,vert_padding,((cur_hour) * width)/12, HEIGHT), 0, 0);

	//set the colour
	graphics_context_set_fill_color(ctx, COLOR_FALLBACK(s_middle_bar_color, GColorGreen));

	//display the minute bar
	graphics_fill_rect(ctx, GRect(PADDING,vert_padding+(HEIGHT + PADDING),(s_min * width)/60, HEIGHT), 0, 0);

	//set the colour
	graphics_context_set_fill_color(ctx, COLOR_FALLBACK(s_bottom_bar_color, GColorBlue));

	//display the second bar
	graphics_fill_rect(ctx, GRect(PADDING,vert_padding + (2*HEIGHT) + (2 * PADDING),(s_sec * width)/60, HEIGHT), 0, 0);
}


static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	s_layer = layer_create(layer_get_bounds(window_get_root_layer(window)));
	layer_add_child(window_get_root_layer(window), s_layer);
	layer_set_update_proc(s_layer, draw_watchface);

	uint16_t vert_padding = (bounds.size.h - ((NUM_BARS - 1)*PADDING) - (NUM_BARS*HEIGHT)) / 2;
	uint16_t width = bounds.size.w - (2 * PADDING); //this is 120 on basalt

	//create the unary ticks image
	s_unary_ticks_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_UNARY_TICKS);
	s_unary_ticks_layer = bitmap_layer_create(GRect(PADDING,vert_padding,width + 1,HEIGHT));
	bitmap_layer_set_background_color(s_unary_ticks_layer, GColorClear);
	bitmap_layer_set_bitmap(s_unary_ticks_layer, s_unary_ticks_bitmap);
	bitmap_layer_set_compositing_mode(s_unary_ticks_layer, GCompOpSet);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_unary_ticks_layer));

	//create the ternary ticks image and layer for minutes
	s_ternary_ticks_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TERNARY_TICKS);
	s_ternary_ticks_layer_minutes = bitmap_layer_create(GRect(PADDING, vert_padding + PADDING + HEIGHT, width + 1, HEIGHT));
	bitmap_layer_set_background_color(s_ternary_ticks_layer_minutes, GColorClear);
	bitmap_layer_set_bitmap(s_ternary_ticks_layer_minutes, s_ternary_ticks_bitmap);
	bitmap_layer_set_compositing_mode(s_ternary_ticks_layer_minutes, GCompOpSet);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_ternary_ticks_layer_minutes));

	//create the ternary ticks layer for seconds
	s_ternary_ticks_layer_seconds = bitmap_layer_create(GRect(PADDING, vert_padding + (2*PADDING) + (2*HEIGHT), width + 1, HEIGHT));
	bitmap_layer_set_background_color(s_ternary_ticks_layer_seconds, GColorClear);
	bitmap_layer_set_bitmap(s_ternary_ticks_layer_seconds, s_ternary_ticks_bitmap);
	bitmap_layer_set_compositing_mode(s_ternary_ticks_layer_seconds, GCompOpSet);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_ternary_ticks_layer_seconds));

	if (persist_read_int(KEY_TOP_BAR_COLOR)) {
		s_top_bar_color = GColorFromHEX(persist_read_int(KEY_TOP_BAR_COLOR));
		///	set_background_and_text_color(background_color);
	} else {
		s_top_bar_color = GColorRed;
	}

	if (persist_read_int(KEY_MIDDLE_BAR_COLOR)) {
		s_middle_bar_color = GColorFromHEX(persist_read_int(KEY_MIDDLE_BAR_COLOR));
	} else {
		s_middle_bar_color = GColorGreen;
	}

	if (persist_read_int(KEY_BOTTOM_BAR_COLOR)) {
		s_bottom_bar_color = GColorFromHEX(persist_read_int(KEY_BOTTOM_BAR_COLOR));
	} else {
		s_bottom_bar_color = GColorBlue;
	}

	if (persist_read_int(KEY_BACKGROUND_COLOR)) {
		set_background_and_text_color(persist_read_int(KEY_BACKGROUND_COLOR));
	} 

	//	if (persist_read_bool(KEY_TWENTY_FOUR_HOUR_FORMAT)) {
	//		twenty_four_hour_format = persist_read_bool(KEY_TWENTY_FOUR_HOUR_FORMAT);
	//	}

	// Create the Bluetooth icon GBitmap
	s_bt_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_ICON);

	// Create the BitmapLayer to display the GBitmap
	s_bt_icon_layer = bitmap_layer_create(GRect(0, 0, 30, 30));
	bitmap_layer_set_bitmap(s_bt_icon_layer, s_bt_icon_bitmap);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bt_icon_layer));

	//show the correct state of the bluetooth connection from the start
#ifdef PBL_SDK_2
	bluetooth_callback(bluetooth_connection_service_peek());
#elif PBL_SDK_3
	bluetooth_callback(connection_service_peek_pebble_app_connection());
#endif

}

static void window_unload(Window *window) {
	gbitmap_destroy(s_unary_ticks_bitmap);
	bitmap_layer_destroy(s_unary_ticks_layer);

	gbitmap_destroy(s_ternary_ticks_bitmap);
	bitmap_layer_destroy(s_ternary_ticks_layer_minutes);
	bitmap_layer_destroy(s_ternary_ticks_layer_seconds);

	//destroy the main layer
	layer_destroy(s_layer);

	//destroy the bluetooth stuffs
	gbitmap_destroy(s_bt_icon_bitmap);
	bitmap_layer_destroy(s_bt_icon_layer);
}

static void init(void) {
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
			.load = window_load,
			.unload = window_unload,
			});
	const bool animated = true;
	window_stack_push(window, animated);

	//Register with TickTimerService
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

	app_message_register_inbox_received(inbox_received_handler);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

	//Register for Bluetooth connections updates
#ifdef PBL_SDK_2
	bluetooth_connection_service_subscribe(bluetooth_callback);
#elif PBL_SDK_3
	connection_service_subscribe((ConnectionHandlers) {
		.pebble_app_connection_handler = bluetooth_callback
		});
#endif

	//display time right away
	time_t start_time = time(NULL);
	update_time(localtime(&start_time));
}

static void deinit(void) {
	tick_timer_service_unsubscribe();
	window_destroy(window);
}

int main(void) {
	init();

	APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

	app_event_loop();
	deinit();
}
