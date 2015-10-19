#include <pebble.h>

static Window *window;
static Layer *s_layer;

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

static void update_time(struct tm * tick_time) {
	s_hour = tick_time->tm_hour;
	s_min = tick_time->tm_min;
	s_sec = tick_time->tm_sec;
	layer_mark_dirty(s_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time(tick_time);
}


static void draw_watchface(Layer *layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(layer);
	uint16_t width = bounds.size.w - (2 * PADDING); //this is 120 on basalt
	uint16_t vert_padding = (bounds.size.h - ((NUM_BARS - 1)*PADDING) - (NUM_BARS*HEIGHT)) / 2;

	//set the colour
	graphics_context_set_fill_color(ctx, COLOR_FALLBACK(GColorSunsetOrange, GColorWhite));

	//display the hour bar
	graphics_fill_rect(ctx, GRect(PADDING,vert_padding,((s_hour % 12) * width)/12, HEIGHT), 0, 0);

	//set the colour
	graphics_context_set_fill_color(ctx, COLOR_FALLBACK(GColorBlueMoon, GColorWhite));

	//display the minute bar
	graphics_fill_rect(ctx, GRect(PADDING,vert_padding+(HEIGHT + PADDING),(s_min * width)/60, HEIGHT), 0, 0);

	//set the colour
	graphics_context_set_fill_color(ctx, COLOR_FALLBACK(GColorJaegerGreen, GColorWhite));

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
	
}

static void window_unload(Window *window) {
	gbitmap_destroy(s_unary_ticks_bitmap);
	bitmap_layer_destroy(s_unary_ticks_layer);

	gbitmap_destroy(s_ternary_ticks_bitmap);
	bitmap_layer_destroy(s_ternary_ticks_layer_minutes);
	bitmap_layer_destroy(s_ternary_ticks_layer_seconds);
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
