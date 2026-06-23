file(GLOB_RECURSE UI_CUSTOM_SRCS
    "custom/*.c"
    "generated/*.c"
)

idf_component_register(
    SRCS ${UI_CUSTOM_SRCS}
    INCLUDE_DIRS    "custom/."
                    "generated/."
                    "generated/guider_customer_fonts"
                    "generated/images"
                    
    REQUIRES lvgl__lvgl  #这里要写自己的lvgl组件名称不然会出现找不到lvgl头文件的报错
)
