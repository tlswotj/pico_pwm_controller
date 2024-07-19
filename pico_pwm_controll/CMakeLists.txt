add_executable(pico_pwm_controller
        pwm_controller.cpp
        )

# pull in common dependencies and additional pwm hardware support
target_link_libraries(pico_pwm_controller pico_stdlib hardware_pwm)
pico_enable_stdio_usb(pico_pwm_controller 1)
pico_enable_stdio_uart(pico_pwm_controller 0)


# create map/bin/hex file etc.
pico_add_extra_outputs(pico_pwm_controller)

# add url via pico_set_program_url
example_auto_set_url(pico_pwm_controller)
