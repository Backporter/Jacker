# Jacker
This will *jack* Several system calls and change thier behavior in a way that is firmware agnositc.

as of now it has two compile modes, XOR / DB, XOR will swap out the default `mov esi, r14d` logic inside SceVideOutSetFlipRate with `xor esi, esi`(when xor recives the same param twice it'll zero out the register, so we use this so we don't use a instruction larger then the oringal) so it ignores the fliprate passed into it and uses 60 FPS mod(0), DB is currently not finished, the idea for it came from @crazyvoid.
