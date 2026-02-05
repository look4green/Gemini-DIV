// ##################################################################################
//                         USER SETUP FOR 2.4" CYD (ESP32-2432S024)
// ##################################################################################

#define USER_SETUP_INFO "CYD_2.4_TFT"

// --- Section 1: Driver ---
#define ILI9341_2_DRIVER     // Working driver for your specific board
// #define TFT_RGB_ORDER TFT_RGB 
#define TFT_RGB_ORDER TFT_BGR  // Most 2.4" CYDs require BGR for correct colors

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// Corrects "inverted" colors (White appearing as Black)
#define TFT_INVERSION_ON

// --- Section 2: Pins for CYD 2.4 ---
#define TFT_BL          27    // Display Backlight
#define TFT_BACKLIGHT_ON HIGH // Backlight is ON when pin is HIGH

#define TFT_MISO        12
#define TFT_MOSI        13
#define TFT_SCLK        14
#define TFT_CS          15    // Chip Select
#define TFT_DC           2    // Data/Command
#define TFT_RST         -1    // RST connected to ESP32 Reset button

#define TOUCH_CS        33    // Touch Controller Chip Select

// --- Section 3: Fonts ---
#define LOAD_GLCD  
#define LOAD_FONT2 
#define LOAD_FONT4 
#define LOAD_FONT6 
#define LOAD_FONT7 
#define LOAD_FONT8 
#define LOAD_GFXFF 
#define SMOOTH_FONT

// --- Section 4: SPI Frequencies ---
// 40MHz is the sweet spot for ILI9341; 27MHz is safer for long wires, 
// but for the integrated CYD, 40MHz provides smoother animations.
#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000
