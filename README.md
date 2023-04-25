# The-theremin---Arduino-Mega-2560

Hello!

This project is based on a musical instrument called Theremin.
The theremin was invented in 1919 (Russia) and it is considered the first electronic and also first contactless instrument of the world.

For this project I replaced antennas with ultrasonic sensors from Elegoo Mega 2560 kit.
Both sensors read the distance from the player's hand and produce notes/effects.
One sensor is intended to change notes from low to high pitch. 
I've included a courtesy RGB led linked to this sensor as well, which goes from cold to hot color following low/high notes.
Second sensor converts the distance from player's hand into RPM that drives the DC motor.
This DC motor has propeller with one single plastic black blade. 
During rotation, the blade passes in front of a LED aligned with a photoresistor. 
The photoresitor is directly connected to the speaker, 
so the black blade creates a kind of "blink" effect for the photoresistor, 
therefore altering its resistance and producing a "tremolo" effect for the notes.

In the current code version, notes were defined as pentatonic scales inside arrays.
It can be fully extended to produce all the 24 possible major and minor scales, as well as its variants like pentatonics or melodic/harmonic minor.
