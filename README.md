# neurc

A small neural network library written from scratch in C.

> **Note: This project is for learning purposes.**
> `neurc` is being written to understand how neural networks work under the hood. It is not designed for production use and makes no guarantees about speed, stability, or API compatibility. For real projects, use mature libraries such as PyTorch or TensorFlow.

## Goals

- Implement the forward pass, backpropagation, and training loop of a neural network step by step, without relying on existing ML libraries
- Practice memory management, data structure design, and clean C library API design
- Eventually make the library usable from Python via `import neurc`

## Status

The project is in an early stage and under active development. The list below is the roadmap; unchecked items are not done yet.

- [ ] Project skeleton and build system (Makefile, static library)
- [ ] Matrix data structure and basic operations
- [ ] Layers and multi-layer forward pass
- [ ] Activation functions (sigmoid, ReLU, etc.)
- [ ] Loss functions
- [ ] Backpropagation and gradient descent
- [ ] XOR example
- [ ] MNIST example
- [ ] Shared library (`.so`/`.dll`) and Python bindings

## Project layout

```
neurc/
├── include/neurc/   public headers
├── src/             implementation
├── tests/           tests
├── examples/        usage examples
├── data/            datasets (not tracked in the repo)
├── Makefile
└── README.md
```

## Building

```
make
```

To clean build artifacts:

```
make clean
```

Requirements: `gcc` and `make`.

## Design principles

- No external dependencies, only the C standard library and `libm`
- All public functions and types are prefixed with `neurc_`
- Library code never calls `exit()`; errors are returned to the caller
- Memory ownership is explicit: every function that creates an object has a matching `free`/`destroy` function

## License

Not decided yet.