#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include "icon.h"
// This configuration is taken directly from the user_setup.h provided by the user.
// It will be used by the TFT_eSPI library to set up the display.
/*
#define ILI9341_2_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 320
#define TFT_BL   27
#define TFT_BACKLIGHT_ON HIGH
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST  -1
#define TOUCH_CS 33
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT
#define SPI_FREQUENCY  27000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000
*/

// TFT instance
TFT_eSPI tft = TFT_eSPI();

// Define EEPROM size for calibration data
#define EEPROM_SIZE 32
const int CAL_DATA_ADDR = 0; // Address to store calibration data

// Placeholder function for the touch calibration routine
void calibrateTouchScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(2);
  tft.println("Touch Calibration");
  tft.println("Please touch the circles as they appear.");

  uint16_t calData[5];

  // Run the built-in calibration
  tft.calibrateTouch(calData, TFT_WHITE, TFT_BLACK, 15);

  Serial.println("Calibration complete.");
  tft.fillScreen(TFT_BLACK);
}


// --- Placeholder Colors and Bitmaps (from shared.h and icon.h) ---
#define BLACK       0x0000
#define WHITE       0xFFFF
// FIX: New color constants based on user's request
#define BUTTON_GRAY   0x2104 // Lighter shade for buttons and status bar background
#define BACKGROUND_GRAY 0x1082 // Darker shade for main screen and footer background
#define ORANGE      0xFC00
#define SELECTED_ICON_COLOR ORANGE

/*
 * * Loading
 * */
void loading(int frameDelay, uint16_t color, int repeats, bool center) {
    int16_t bW = 100; int16_t bH = 120;
    int16_t lX = (tft.width() - bW) / 2;
    int16_t lY = (tft.height() - bH) / 2;

    const unsigned char* bitmaps[] = {
        bitmap_icon_skull_loading_1, bitmap_icon_skull_loading_2,
        bitmap_icon_skull_loading_3, bitmap_icon_skull_loading_4,
        bitmap_icon_skull_loading_5, bitmap_icon_skull_loading_6,
        bitmap_icon_skull_loading_7, bitmap_icon_skull_loading_8,
        bitmap_icon_skull_loading_9, bitmap_icon_skull_loading_10
    };

    for (int r = 0; r < repeats; r++) {
        for (int i = 0; i < 10; i++) {
            tft.fillRect(lX, lY, bW, bH, BACKGROUND_GRAY);
            tft.drawBitmap(lX, lY, bitmaps[i], bW, bH, color);
            delay(frameDelay);
        }
    }
}


void drawStatusBar() {
  // FIX: Status bar background color now matches the buttons
  tft.fillRect(0, 0, tft.width(), 20, BUTTON_GRAY);

  // Draw a thin line to separate the status bar from the content
  tft.drawLine(0, 20, tft.width(), 20, BUTTON_GRAY);

  // Draw GEMINI V1.1 text
  tft.setTextColor(WHITE, BUTTON_GRAY);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setCursor(5, 5);
  tft.print("BlackWire V1.1");

  // Draw signal strength icon
  int signalX = tft.width() - 35;
  int signalY = 5;
  tft.fillRect(signalX, signalY + 5, 2, 5, WHITE);
  tft.fillRect(signalX + 5, signalY + 3, 2, 7, WHITE);
  tft.fillRect(signalX + 10, signalY + 1, 2, 9, WHITE);
  tft.fillRect(signalX + 15, signalY, 2, 11, WHITE);

  // Draw SD card icon
  int sdX = tft.width() - 15;
  int sdY = 5;
  tft.drawRect(sdX, sdY, 15, 10, WHITE);
  tft.fillRect(sdX + 2, sdY, 11, 2, WHITE);
}

void drawFooter() {
    // FIX: Footer background color now matches the main screen background
    tft.setTextColor(WHITE, BACKGROUND_GRAY);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.fillRect(0, tft.height() - 20, tft.width(), 20, BACKGROUND_GRAY); // Moved up
    int textWidth = tft.textWidth("Tailored For CYD");
    int textX = (tft.width() - textWidth) / 2;
    int textY = tft.height() - 20; // Centered vertically, moved up slightly
    tft.setCursor(textX, textY);
    tft.print("Tailored For CYD");
}


float readBatteryVoltage() {
  // This is a placeholder function.
  // In a real application, this would read an ADC pin.
  return 4.2;
}

void displayLogo(uint16_t color, unsigned long duration) {
  tft.fillScreen(BLACK);
  tft.setTextColor(color);
  tft.setTextSize(3);
  tft.setCursor(50, 100);
  tft.print("BlackWire");
  delay(duration);
}

void handleAboutPage() {
    // FIX: Changed background to BACKGROUND_GRAY to match the main menu
    tft.fillScreen(BACKGROUND_GRAY);
    
    // --- START OF FIX ---
    // Instead of a button, draw a link that looks like the submenu items
    int linkX = 10;
    int linkY = 30;
    int linkWidth = tft.width() - 20;
    int linkHeight = 25;

    // Draw the icon
    tft.drawBitmap(linkX, linkY + (linkHeight - 16) / 2, bitmap_icon_go_back, 16, 16, WHITE, BACKGROUND_GRAY);

    // Draw the text
    tft.setTextColor(WHITE, BACKGROUND_GRAY);
    tft.setCursor(linkX + 20, linkY + (linkHeight - 8) / 2); // Adjusted x-position to accommodate the icon
    tft.print("| Back to Main Menu");
    
    // Set the touch area for the link
    int buttonX = linkX;
    int buttonY = linkY;
    int buttonWidth = linkWidth;
    int buttonHeight = linkHeight;
    // --- END OF FIX ---
    
    // Set text properties for the content
    tft.setTextColor(TFT_GREEN, BACKGROUND_GRAY);
    tft.setTextSize(1);
    tft.setTextFont(2);

    const char* title = "[About This Project]";
    tft.setCursor(10, 70);
    tft.println(title);

    int lineHeight = 30;
    int text_x = 10;
    int text_y = 100; // Adjusted starting Y position
    tft.setCursor(text_x, text_y);
    tft.println("GEMINI");
    text_y += lineHeight;
    tft.setCursor(text_x, text_y);
    tft.println("- Developed by: KEJU");
    text_y += lineHeight;
    tft.setCursor(text_x, text_y);
    tft.println("- Version: 1.1");
    text_y += lineHeight;
    tft.setCursor(text_x, text_y);
    tft.println("- Contact: realkeju@icloud.com");
    text_y += lineHeight;
    tft.setCursor(text_x, text_y);
    tft.println("- GitHub: github.com/to be edited");
    text_y += lineHeight;
    tft.setCursor(text_x, text_y);
    tft.println("- Website: HackThePlanet.net");
    text_y += lineHeight;
    
    drawStatusBar(); // Ensure status bar is the last thing drawn

    while(true) {
        uint16_t touchX, touchY;
        if (tft.getTouch(&touchX, &touchY)) {
             // START OF NEW FEATURE: Visual feedback on touch
            tft.drawBitmap(linkX, linkY + (linkHeight - 16) / 2, bitmap_icon_go_back, 16, 16, ORANGE, BACKGROUND_GRAY);
            tft.setTextColor(ORANGE, BACKGROUND_GRAY);
            tft.setCursor(linkX + 20, linkY + (linkHeight - 8) / 2);
            tft.print("| Back to Main Menu");
            delay(100); // Wait briefly
            tft.drawBitmap(linkX, linkY + (linkHeight - 16) / 2, bitmap_icon_go_back, 16, 16, WHITE, BACKGROUND_GRAY);
            tft.setTextColor(WHITE, BACKGROUND_GRAY);
            tft.setCursor(linkX + 20, linkY + (linkHeight - 8) / 2);
            tft.print("| Back to Main Menu");
            // END OF NEW FEATURE
            if (touchX >= buttonX && touchX <= buttonX + buttonWidth && touchY >= buttonY && touchY <= buttonY + buttonHeight) {
                break;
            }
        }
        delay(10);
    }
}


// --- Menu and Submenu definitions ---
bool feature_active = false;
bool feature_exit_requested = false;

const int NUM_MENU_ITEMS = 4;
const char *menu_items[NUM_MENU_ITEMS] = {
    "WiFi",
    "Bluetooth",
    "IR Remote",
    "About"
};

const unsigned char *bitmap_icons[NUM_MENU_ITEMS] = {
    bitmap_icon_wifi,
    bitmap_icon_spoofer,
    bitmap_icon_jammer,
    bitmap_icon_analyzer
};

int current_menu_index = 0;
bool is_main_menu = true; // Start in main menu

const int NUM_WIFI_SUBMENU_ITEMS = 8;
const char *wifi_submenu_items[NUM_WIFI_SUBMENU_ITEMS] = {
    "Touch Test", // Added touch test here
    "Packet Monitor",
    "Beacon Spammer",
    "WiFi Deauther",
    "Deauth Detector",
    "WiFi Scanner",
    "Captive Portal",
    "Back to Main Menu"
};

const int bluetooth_NUM_SUBMENU_ITEMS = 6;
const char *bluetooth_submenu_items[bluetooth_NUM_SUBMENU_ITEMS] = {
    "BLE Jammer",
    "BLE Spoofer",
    "Sour Apple",
    "Sniffer",
    "BLE Scanner",
    "Back to Main Menu"
};

const int ir_NUM_SUBMENU_ITEMS = 3;
// FIX: Changed ir_SUBMENU_ITEMS to ir_NUM_SUBMENU_ITEMS to fix the compilation error
const char *ir_submenu_items[ir_NUM_SUBMENU_ITEMS] = {
    "Record [Coming soon]",
    "Saved Profile [Coming soon]",
    "Back to Main Menu"
};

int current_submenu_index = 0;
bool in_sub_menu = false;

const char **active_submenu_items = nullptr;
int active_submenu_size = 0;

const unsigned char *wifi_submenu_icons[NUM_WIFI_SUBMENU_ITEMS] = {
    bitmap_icon_analyzer,
    bitmap_icon_wifi,       // Packet Monitor
    bitmap_icon_antenna,    // Beacon Spammer
    bitmap_icon_wifi_jammer,  // WiFi Deauther
    bitmap_icon_eye2,       // Deauth Detector
    bitmap_icon_jammer,     // WiFi Scanner
    bitmap_icon_bash,       // Captive Portal
    bitmap_icon_go_back
};

const unsigned char *bluetooth_submenu_icons[bluetooth_NUM_SUBMENU_ITEMS] = {
    bitmap_icon_ble_jammer,   // BLE Jammer
    bitmap_icon_spoofer,      // BLE Spoofer
    bitmap_icon_apple,      // Sour Apple
    bitmap_icon_analyzer,   // Analyzer
    bitmap_icon_graph,      // BLE Scanner
    bitmap_icon_go_back
};

const unsigned char *ir_submenu_icons[ir_NUM_SUBMENU_ITEMS] = {
    bitmap_icon_question,
    bitmap_icon_question,
    bitmap_icon_go_back
};

const unsigned char **active_submenu_icons = nullptr;

void updateActiveSubmenu() {
    switch (current_menu_index) {
        case 0: // WiFi
            active_submenu_items = wifi_submenu_items;
            active_submenu_size = NUM_WIFI_SUBMENU_ITEMS;
            active_submenu_icons = wifi_submenu_icons;
            break;
        case 1: // Bluetooth
            active_submenu_items = bluetooth_submenu_items;
            active_submenu_size = bluetooth_NUM_SUBMENU_ITEMS;
            active_submenu_icons = bluetooth_submenu_icons;
            break;
        case 2: // IR
            active_submenu_items = ir_submenu_items;
            active_submenu_size = ir_NUM_SUBMENU_ITEMS;
            active_submenu_icons = ir_submenu_icons;
            break;
        case 3: // About
            active_submenu_items = nullptr;
            active_submenu_size = 0;
            active_submenu_icons = nullptr;
            break;
        default:
            active_submenu_items = nullptr;
            active_submenu_size = 0;
            active_submenu_icons = nullptr;
            break;
    }
}

float currentBatteryVoltage = readBatteryVoltage();
unsigned long last_interaction_time = 0;

int last_submenu_index = -1;
bool submenu_initialized = false;
int last_menu_index = -1;
bool menu_initialized = false;

// --- Touch Test Feature ---
void runTouchTest() {
  // FIX: Main screen background is now BACKGROUND_GRAY
  tft.fillScreen(BACKGROUND_GRAY);
  tft.setCursor(10, 30);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.println("Touch Test Active");
  tft.println("Touch the screen to see coordinates.");
  tft.println("\nPress 'Back' to return to menu.");

  drawStatusBar(); // Ensure status bar is drawn on top

  while (feature_active) {
    uint16_t x, y;
    if (tft.getTouch(&x, &y)) {
      // FIX: Clear previous text dynamically with BACKGROUND_GRAY
      tft.fillRect(10, 60, tft.width() - 20, 40, BACKGROUND_GRAY);
      tft.setCursor(10, 60);
      tft.printf("Touch at: X=%d Y=%d\n", x, y);
      Serial.printf("Touch detected at: X=%d Y=%d\n", x, y);
      delay(200); // Debounce
    }
    // Check for exit condition (e.g., a "back" button press)
    if (feature_exit_requested) {
      feature_active = false;
      feature_exit_requested = false;
    }
    delay(100);
  }
}

// Universal function to handle all submenu button presses
void handleSubmenuButtons() {
    uint16_t x, y;
    if (tft.getTouch(&x, &y) && !feature_active) {
        for (int i = 0; i < active_submenu_size; i++) {
            int yPos = 30 + i * 30;
            if (i == active_submenu_size - 1) {
                yPos += 10;
            }
            int button_x1 = 10;
            int button_y1 = yPos;
            int button_x2 = tft.width() - 10;
            int button_y2 = button_y1 + 25;
            if (x >= button_x1 && x <= button_x2 && y >= button_y1 && y <= button_y2) {
                current_submenu_index = i;
                last_interaction_time = millis();
                
                // START OF NEW FEATURE: Visual feedback
                tft.drawBitmap(10, yPos, active_submenu_icons[i], 16, 16, ORANGE, BACKGROUND_GRAY);
                tft.setTextColor(ORANGE, BACKGROUND_GRAY);
                tft.setCursor(30, yPos);
                if (i < active_submenu_size - 1) tft.print("| ");
                tft.print(active_submenu_items[i]);
                delay(100); // Flash for 100ms
                tft.drawBitmap(10, yPos, active_submenu_icons[i], 16, 16, WHITE, BACKGROUND_GRAY);
                tft.setTextColor(WHITE, BACKGROUND_GRAY);
                tft.setCursor(30, yPos);
                if (i < active_submenu_size - 1) tft.print("| ");
                tft.print(active_submenu_items[i]);
                // END OF NEW FEATURE
                
                displaySubmenu();
                delay(200);
                if (active_submenu_items[current_submenu_index] == "Back to Main Menu") {
                    in_sub_menu = false;
                    feature_active = false;
                    feature_exit_requested = false;
                    is_main_menu = true;
                    displayMenu();
                } else if (active_submenu_items[current_submenu_index] == "Touch Test") {
                    feature_active = true;
                    runTouchTest();
                    displaySubmenu(); // Redraw menu after test
                } else {
                    feature_active = true;
                    // Placeholder for other features
                    // FIX: Changed background to BACKGROUND_GRAY for consistency
                    tft.fillScreen(BACKGROUND_GRAY);
                    tft.setCursor(10, 30);
                    tft.setTextColor(TFT_WHITE);
                    tft.setTextFont(2);
                    tft.setTextSize(1);
                    tft.printf("%s selected. Not yet implemented.\n", active_submenu_items[current_submenu_index]);
                    tft.println("Touch anywhere to go back.");
                    drawStatusBar();
                    while (true) {
                        uint16_t touchX, touchY;
                        if (tft.getTouch(&touchX, &touchY)) {
                            break;
                        }
                        delay(10);
                    }
                    feature_active = false;
                    displaySubmenu();
                }
                break;
            }
        }
    }
}


void displaySubmenu() {
    menu_initialized = false;
    last_menu_index = -1;

    // FIX: Main screen background is now BACKGROUND_GRAY
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.fillScreen(BACKGROUND_GRAY);

    for (int i = 0; i < active_submenu_size; i++) {
        int yPos = 30 + i * 30;
        if (i == active_submenu_size - 1) yPos += 10;
        
        // FIX: Text background color now matches the main screen background
        tft.setTextColor((i == active_submenu_size - 1) ? WHITE : WHITE, BACKGROUND_GRAY);
        tft.drawBitmap(10, yPos, active_submenu_icons[i], 16, 16, (i == active_submenu_size - 1) ? WHITE : WHITE);
        tft.setCursor(30, yPos);
        if (i < active_submenu_size - 1) {
            tft.print("| ");
        }
        tft.print(active_submenu_items[i]);
    }
    submenu_initialized = true;
    last_submenu_index = -1;
    
    drawStatusBar(); // Draw status bar last
}

void displayMenu() {
    submenu_initialized = false;
    last_submenu_index = -1;
    tft.setTextFont(2);
    tft.setTextSize(1);

    // FIX: Main screen background is now BACKGROUND_GRAY
    tft.fillScreen(BACKGROUND_GRAY);

    int icon_size = 32;
    int button_height = icon_size + 20;
    int y_start = 30;
    int y_spacing = button_height + 15;

    for (int i = 0; i < NUM_MENU_ITEMS; i++) {
        int x_position = 10;
        int y_position = y_start + i * y_spacing;
        int button_width = tft.width() - 20;

        // FIX: Button background is now BUTTON_GRAY
        tft.fillRoundRect(x_position, y_position, button_width, button_height, 5, BUTTON_GRAY);
        tft.drawRoundRect(x_position, y_position, button_width, button_height, 5, WHITE);

        // Draw icon on the left
        tft.drawBitmap(x_position + 10, y_position + (button_height - 16) / 2, bitmap_icons[i], 16, 16, WHITE);

        // Draw text centered
        // FIX: Button text background is now BUTTON_GRAY
        tft.setTextColor(WHITE, BUTTON_GRAY);
        int textWidth = 6 * strlen(menu_items[i]);
        int textX = x_position + (button_width - textWidth) / 2;
        int textY = y_position + (button_height - 8) / 2; // Adjust for font height
        tft.setCursor(textX, textY);
        tft.print(menu_items[i]);
    }
    
    menu_initialized = true;
    last_menu_index = -1;
    
    drawFooter();
    drawStatusBar(); // Draw status bar last
}


void handleButtons() {
    static unsigned long lastTouchTime = 0;
    const unsigned long touchFeedbackDelay = 100;
    uint16_t x, y;

    if (tft.getTouch(&x, &y) && (millis() - lastTouchTime >= touchFeedbackDelay)) {
        lastTouchTime = millis();
        int icon_size = 32;
        int button_height = icon_size + 20;
        int y_start = 30;
        int y_spacing = button_height + 15;
        int button_width = tft.width() - 20;
        int x_position = 10;

        for (int i = 0; i < NUM_MENU_ITEMS; i++) {
            int y_position = y_start + i * y_spacing;
            int button_x1 = x_position;
            int button_y1 = y_position;
            int button_x2 = x_position + button_width;
            int button_y2 = y_position + button_height;

            if (x >= button_x1 && x <= button_x2 && y >= button_y1 && y <= button_y2) {
                current_menu_index = i;
                last_interaction_time = millis();
                
                // START OF NEW FEATURE: Visual feedback on touch
                // Draw the orange outline.
                tft.drawRoundRect(x_position, y_position, button_width, button_height, 5, ORANGE);
                
                // Draw the orange text. Note the background color is the same as the button's fill.
                tft.setTextColor(ORANGE, BUTTON_GRAY);
                int textWidth = 6 * strlen(menu_items[i]);
                int textX = x_position + (button_width - textWidth) / 2;
                int textY = y_position + (button_height - 8) / 2;
                tft.setCursor(textX, textY);
                tft.print(menu_items[i]);
                
                // Highlight the icon in orange
                tft.drawBitmap(x_position + 10, y_position + (button_height - 16) / 2, bitmap_icons[i], 16, 16, ORANGE);
                
                delay(100); // Wait briefly
                
                // Redraw the button to its original state.
                tft.drawRoundRect(x_position, y_position, button_width, button_height, 5, WHITE);
                tft.setTextColor(WHITE, BUTTON_GRAY);
                tft.setCursor(textX, textY);
                tft.print(menu_items[i]);
                
                // Restore the icon to its original white color
                tft.drawBitmap(x_position + 10, y_position + (button_height - 16) / 2, bitmap_icons[i], 16, 16, WHITE);
                // END OF NEW FEATURE

                // Handle main menu button presses
                if (current_menu_index == 3) {
                    // About page
                    handleAboutPage();
                    displayMenu(); // Return to main menu after about page
                } else {
                    // Go to a submenu
                    updateActiveSubmenu();
                    if (active_submenu_items && active_submenu_size > 0) {
                        current_submenu_index = 0;
                        in_sub_menu = true;
                        submenu_initialized = false;
                        displaySubmenu();
                    }
                }
                delay(200);
                break;
            }
        }
    }
}


void setup() {
    Serial.begin(115200);
    tft.init();
    tft.setRotation(0); // Set to landscape mode to fix layout issues
    // The initial screen fill is now set to black.
    tft.fillScreen(TFT_BLACK);
    // Use the new loading function here. The parameters are (frameDelay, color, x, y, repeats, center)
    loading(80, ORANGE, 3, true);
    tft.fillScreen(BACKGROUND_GRAY);
    displayLogo(WHITE, 2000);
    tft.setTextSize(1); // FIX: Set text size back to normal after the logo
    displayMenu();
    last_interaction_time = millis();
}

void loop() {
    // Check if we are in a submenu or the main menu
    if (in_sub_menu) {
        handleSubmenuButtons();
    } else {
        handleButtons();
    }
}
