#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include <ws2811/ws2811.h>

#include "coords.h"
#include "leds.h"
#include "random.h"
#include "version.h"
#include "effects/effect.h"

#define GPIO_PIN 18
#define DMA_CHANNEL 10
#define BRIGHTNESS 50

// LED strip
static struct ws2811_t leds = {
    .freq = WS2811_TARGET_FREQ,
    .dmanum = DMA_CHANNEL,
    .channel = {
        [0] = {
            .gpionum = GPIO_PIN,
            .invert = 0,
            .count = 0,
            .strip_type = WS2811_STRIP_RGB,
            .brightness = BRIGHTNESS,
        },
        [1] = {
            .gpionum = 0,
            .invert = 0,
            .count = 0,
            .strip_type = WS2811_STRIP_RGB,
            .brightness = 0
        }
    }
};

// Running status
static unsigned char running = 1;

// Termination handler
static void _int_handler(int signum) {
    // Bypass unused variable warnings
    (void) signum;

    // Just modify the running flag
    running = 0;
}

// Inject termination handler
static int _setup_term_handler(void) {
    struct sigaction action = {
        .sa_handler = &_int_handler
    };

    // Set signal handlers
    if (sigaction(SIGINT, &action, NULL)) {
        fprintf(stderr, "Could not set SIGINT handler: %s\n", strerror(errno));
        return 1;
    }
    if (sigaction(SIGTERM, &action, NULL)) {
        fprintf(stderr, "Could not set SIGTERM handler: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    // Usage
    if (argc != 2) {
        fprintf(stderr, "Usage: %s\n <coords>", argv[0]);
        return -1;
    }

    // Program header
    // Colors UwU
    printf("\x1B[36m----------=[*]=[ LEDPI ]=[*]=----------\n");
    printf("Version: %s\n", LEDPI_VERSION);
    printf("Author: %s\n", LEDPI_AUTHOR);
    printf("---------------------------------------\x1B[0m\n");

    // RNG
    printf("Initialising RNG\n");
    random_init();

    // Vector list
    printf("Loading coordinate list from %s\n", argv[1]);
    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        fprintf(stderr, "Could not open coordinate list: %s\n", strerror(errno));
        return -1;
    }
    struct vectors_t *vectors = vectors_read(file);
    if (fclose(file)) {
        if (vectors) {
            vectors_destroy(vectors);
        }
        fprintf(stderr, "Could not close coordinate list: %s\n", strerror(errno));
        return -1;
    }
    if (!vectors) {
        return -1;
    }
    leds.channel[0].count = vectors->size;
    printf("Loaded %d LEDs\n", vectors->size);

    // Cleanup on exit
    printf("Injecting termination handler\n");
    _setup_term_handler();

    // LED strip initialisation
    printf("Initialising LEDs\n");
    ws2811_return_t result = ws2811_init(&leds);
    if (result != WS2811_SUCCESS) {
        fprintf(stderr, "Could not initialize LEDs: %s\n", ws2811_get_return_t_str(result));
        vectors_destroy(vectors);
        return -1;
    }
    printf("Creating LED wrapper\n");
    struct led_strip_t *wrapper = leds_wrap(&leds);
    if (!wrapper) {
        fprintf(stderr, "Could not initialize LED wrapper\n");
        vectors_destroy(vectors);
        ws2811_fini(&leds);
        return -1;
    }

    // Set vectors
    printf("Injecting LED vector list\n");
    for (int i = 0; i < *(wrapper->size); i++) {
        wrapper->leds[i].position = &(vectors->array[i]);
    }

    // Effect loop
    printf("Running effect\n");
    run(&running, wrapper);

    // Clearing LEDs
    printf("Clearing LEDs\n");
    leds_clear(wrapper);
    leds_render(wrapper);

    // Shutdown
    printf("Finalising LEDs\n");
    leds_unwrap(wrapper);
    ws2811_fini(&leds);
    vectors_destroy(vectors);

    printf("Exiting LEDPI\n");
    return 0;
}
