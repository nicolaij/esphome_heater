import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import display, spi
from esphome.const import CONF_ID, CONF_CS_PIN, CONF_RESET_PIN, CONF_LAMBDA, CONF_PAGES

DEPENDENCIES = ['spi']

ST7920_spi = cg.esphome_ns.namespace('ST7920_spi')
ST7920Component = ST7920_spi.class_('ST7920Component', cg.PollingComponent, display.DisplayBuffer, spi.SPIDevice)

CONFIG_SCHEMA = display.FULL_DISPLAY_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ST7920Component),
    cv.Optional(CONF_RESET_PIN): pins.gpio_output_pin_schema, 
    cv.Optional(CONF_CS_PIN): pins.gpio_output_pin_schema,
}).extend(cv.polling_component_schema('5s')).extend(spi.spi_device_schema())


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield spi.register_spi_device(var, config)
    yield display.register_display(var, config) 

    if CONF_RESET_PIN in config:
        reset = yield cg.gpio_pin_expression(config[CONF_RESET_PIN])
        cg.add(var.set_reset_pin(reset))

    if CONF_LAMBDA in config:
        lambda_ = yield cg.process_lambda(config[CONF_LAMBDA], [(display.DisplayBufferRef, 'it')], return_type=cg.void)
        cg.add(var.set_writer(lambda_))
