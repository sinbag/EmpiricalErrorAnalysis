# Empirical Error Analysis (EEA)

This repository contains the variance/error/Fourier analysis code developed as part of the SIGGRAPH course

> **Fourier analysis of numerical integration in Monte Carlo rendering: Theory and Practice**<br/>
> Kartic Subr, Gurprit Singh, Wojciech Jarosz<br/>
> In *SIGGRAPH Courses July 2016*<br/>
> [Project page](https://www.cs.dartmouth.edu/~wjarosz/publications/subr16fourier.html)

If you use this source code in your research, please cite the code using the following entry:

> @inproceedings{subr16fourier,<br/>
>    author = {Kartic Subr and Gurprit Singh and Wojciech Jarosz},<br/>
>    title = {Fourier Analysis of Numerical Integration in Monte Carlo Rendering: Theory and Practice},<br/>
>    booktitle = {ACM SIGGRAPH Courses},<br/>
>    year = {2016},<br/>
>    month = jul,<br/>
>    publisher = {ACM},<br/>
>    address = {New York, NY, USA},<br/>
>    location = {Anaheim, California},<br/>
>    isbn = {978-1-4503-4289-6},<br/>
>    doi = {10.1145/2897826.2927356}<br/>
>}

## Dependencies
* List of dependencies:
 * CGAL is made optional. If you happen to use integrands that depend on CGAL (PWConstIntegrand & QPIntegrand) then use `set(MYCGAL_ENABLED 1)` in the CMakeLists.txt and [Install CGAL](http://www.cgal.org/download.html) (only [Main library](http://doc.cgal.org/latest/Manual/installation.html#seclibraries) required with basic functionalities) prior to `cmake..`. 
 * GMP (While installing CGAL, GMP gets installed as a dependency)
 * TBB (for parallel computation of Fourier analysis) made optional for those who are only interested in the variance/mse analysis. Simply set TBB_ENABLED flag to zero in the CMakeLists.txt file to disable TBB. After that, you don't even need an installation of TBB on your system. 
 * Python (only required for PBRTIntegrand, the source code still compiles and run for other integrands without Python)
 
## Compiling
* Use recursive tag to clone from git to add the Openexr submodule:
```
git clone --recursive https://github.com/sinbag/EmpiricalErrorAnalysis.git
cd EmpiricalErrorAnalysis
mkdir build
cd build
cmake ..
make -j4
```
* If you accidentally already cloned the repo without the recursive tag, run the following command to also fetch the OpenEXR dependency:
```
git submodule update --init --recursive
```

## Usage
* The interface supports different types of integrand. You can hit:
```
./build/eea or ./build/eea -h
```
to see the command line usage. Example to call Disk Integrand for variance analysis is as follows:
```
./build/exec -S --stype Jittered -I --itype Disk --rad 0.25 --center 0.5 0.5 -A --atype var --nsamps  9 36 100 512  --nreps 1000 -G --ofile testDisk
```
* Detailed description of all the parameters is added in the recent version of the course notes provided on the [project page](https://www.cs.dartmouth.edu/~wjarosz/publications/subr16fourier.html). 

**Pbrt-v3 Error Analyzer**

* The interface also allow user to call PBRT-v3 code directly via python script (Make sure you have a cropwindow defined in the .pbrt scene file to select the region you are interested in):
  * To perform Variance analysis for pbrt-v3 generated images, user can directly call pbrt from the provided Analysis code (look for PBRTIntegrand in the code). Make sure you have a cropwindow defined in the .pbrt scene file to select the region you are interested in. Variance is computed in an online fashion without any reference image. To save time use `--refnspp 1`.
 Example to call PBRTIntegrand (all in one line):
```
./build/eea -S --stype Random 
-I --itype Pbrt 
--epath path-to-pbrt-v3-executable/pbrt 
--spath path-to-pbrt-v3-scenes/anim-killeroos.pbrt 
--pypath path-to-FAS2016/code/Analysis/python/pbrt-cl.py 
--crop 0.25 0.75 0.25 0.75 
--pbrtstype stratified
--refnspp 1
--img pbrt-eea.exr 
-A --atype var --nsamps 1 4 16 64 256 1024 --nreps 200 
-G --ofile pbrt-killeroos
```
 * The Pbrt Integrand directly calls the integrators that are called from the `SamplerIntegrator::Render(const Scene &scene){...}` function. That includes: `directlighting, whitted, path, volpath` integrators. However, it would be straight forward to use this error analysis source code for other integrators (with minor tweaks by the user).
  * Note: In Pbrt, the seed for random number generator for all samplers is set to be deterministic. As a result, for each call to Pbrt the same set of random numbers will be generated resulting in zero variance. To obtain variance we need to randomize the seed, one way to randomize the seed is by using the following snippet in the function call `SamplerIntegrator::Render(const Scene &scene){...}` in the `integrator.cpp`file :
 
```
...
ProgressReporter reporter(nTiles.x * nTiles.y, "Rendering");
    {
        //StatTimer timer(&renderingTime);
        ParallelFor2D([&](Point2i tile){
...

std::random_device rd;
static thread_local std::mt19937 generator(rd());
std::uniform_int_distribution<uint32_t> dis(0, std::numeric_limits<uint32_t>::max());
unsigned int randomseed = dis(generator);

...

///comment out lrandseed to use default PBRT seed.
int seed = tile.y * nTiles.x + tile.x + randomseed;
...
 ```
* Note also that, currently if the user wants to perform convergence analysis for one pixel, pbrt-v3 does not provide direct flexibility to choose one pixel but rather a range of pixels through crop window size in `[0,1]`. We recommend not to use this default `[0,1]` crop window sizing that comes within pbrt-v3 repo. User should update the crop window intake (in `pbrt/src/core/film.cpp`) to actual pixel coordinate to get correct results, e.g. if you want to test pixel `(277,256)` then the crop window coordinates passed to EEA command line should look like this: `--crop 277 278 256 257`. This would also require changing the `tilesize = 1` in the `void Render(const Scene &scene){...}` call function in original pbrt source code, to avoid pixel counter going out of range. Special attention must also be given while choosing pixels that are near boundaries. One quick fix that we found is to update the sampleBounds to: `Bounds2i sampleBounds = camera->film->croppedPixelBounds;` which works well.

* MSE analyzer works the same as variance analyzer but you need a reference value. For pbrt-v3, you need to compute the reference image (ReferenceSampler used Halton) with huge number of samples per pixel (at least four times the maxiumum sample count used to render given pixel, e.g. `--refnspp 4096`).  
Example to call PBRTIntegrand (all in one line):
```
./build/eea -S --stype Random 
-I --itype Pbrt 
--epath path-to-pbrt-v3-executable/pbrt 
--spath path-to-pbrt-v3-scenes/anim-killeroos.pbrt 
--pypath path-to-FAS2016/code/Analysis/python/pbrt-cl.py 
--crop 0.25 0.75 0.25 0.75 
--pbrtstype stratified
--refnspp 4096
--img pbrt-eea.exr 
-A --atype var --nsamps 1 4 16 64 256 1024 --nreps 200 
-G --ofile pbrt-killeroos
```
