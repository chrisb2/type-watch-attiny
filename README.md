# type-watch
This project is based on an [ATtiny85](https://www.microchip.com/en-us/product/ATtiny85) MCU and uses two piezoelectric sensors under a keyboard to sense harsh typing and flashes a LED when a strong keystroke is detected. This feedback allows the user to train themselves to type gently, thus avoiding occupational overuse syndrome (OOS).

The program can use any MCU which supports the Arduino language.

The piezo sensors are [Sparkfun Piezo Elements](https://www.sparkfun.com/products/10293) and are connected at connector _J3_.

![circuit diagram](images/type-watch-circuit.png)

The piezoelectric sensors are place under each foot of the keyboard.
