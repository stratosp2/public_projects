/* FUNCTIONS DECLARATION */
uint32_t read_distance(File&);
uint32_t initialize_SD();
BMPdata get_baseline();
void write_distance(const uint32_t&);
void reset_distance(uint32_t&);
void print_logo();
void print_table();
void print_data(const GPSdata&, const BMPdata&, GPSdata&, BMPdata&);
void update_battery_state(const uint16_t&, uint8_t&, Bat_data&);
void color_GPS_icon(uint32_t&);
void flip_CS();
void initialize_GPS();
void initialize_BMP();
void initialize_TFT();
void read_BMP(const BMPdata&, BMPdata&);
bool read_GPS(GPSdata&);
void update_SD(const GPSdata&, const BMPdata&, const Bat_data&);
void update_DIST(GPSdata&, GPSdata&);
void debugging_USB(bool, bool, bool, bool, bool, const GPSdata&, const BMPdata&);
void draw_arc(int16_t, int16_t, uint8_t, uint16_t, uint16_t, uint16_t);
void timeout(TIMER&, void (*action)(uint32_t&), uint32_t&);
void IRAM_ATTR Ext_INT1_ISR();

