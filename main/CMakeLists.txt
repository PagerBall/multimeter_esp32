idf_component_register(
    SRCS
        "main.c"
        "lcd.c"
        "ui.c"
        "button.c"
        "gpio.c"
        "iic.c"
        "user_timer.c"
        "bat_pcnt.c"
        "ads1115.c"

       
    INCLUDE_DIRS
        "."

    
    REQUIRES ui_custom
)
