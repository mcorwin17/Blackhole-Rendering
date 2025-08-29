# Blackhole-Rendering

a c++ raytracer that renders black holes with gravitational lensing

## what it does

- simulates light bending around massive objects
- renders accretion disk with temperature effects  
- basic schwarzschild metric implementation
- outputs to ppm format

## building

```bash
make
./blackhole
```

## output

generates `black_hole.ppm` - you can view it with most image editors or convert to png:

```bash
convert black_hole.ppm black_hole.png
```

## physics

uses simplified general relativity calculations for:
- event horizon rendering
- gravitational ray bending  
- orbital velocity effects on disk coloring

not 100% accurate but gives a decent approximation of what you'd actually see

## features
- realistic schwarzschild metric calculations
- adaptive ray marching for performance
- temperature-based accretion disk coloring

## todo

- [ ] add better anti-aliasing
- [ ] implement kerr metric for rotating black holes
- [ ] add real-time rendering option
- [ ] better disk texture/detail

## performance
renders 800x600 in ~30-60 seconds on modern hardware
