#!/bin/sh

#  run-batch.sh
#  
#
#  Created by Gurpreet Singh Bagga on 26/03/17.
#

./build/eea -S --stype Random -I --itype Pbrt --epath ./../pbrtv3-linesamplinganalysis/build/pbrt --spath ./../pbrtv3-linesamplinganalysis/scenes/sponza-direct.pbrt --pypath python/light-sampling.py --crop 410 411 273 274 --pbrtstype random --refnspp 1 --img pbrt-eea.exr -A --atype var --nsamps 1 4 16 64 256 1024 16384 --nreps 100 -G --ofile sponza-direct-line-random-nspp16-x410-y273

./build/eea -S --stype Random -I --itype Pbrt --epath ./../pbrtv3-linesamplinganalysis/build/pbrt --spath ./../pbrtv3-linesamplinganalysis/scenes/sponza-direct.pbrt --pypath python/light-sampling.py --crop 410 411 273 274 --pbrtstype regular --refnspp 1 --img pbrt-eea.exr -A --atype var --nsamps 1 4 16 64 256 1024 16384 --nreps 100 -G --ofile sponza-direct-line-regular-nspp16-x410-y273