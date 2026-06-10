libcint Integration
===================

.. contents:: Table of Contents
   :local:
   :depth: 2

Overview
--------

GBasis now includes the `libcint <https://github.com/sunqm/libcint>`_ C library as a backend
for efficient integral evaluation. This integration was added as part of
`GSoC 2026 Issue #229 <https://github.com/theochem/gbasis/issues/229>`_.

libcint is a highly optimized C library for evaluating Gaussian-type orbital (GTO) integrals.
It supports a rich set of 1-electron, 2-electron, and 3-center integrals.

On hardware platforms that support AVX2 (x86_64), GBasis automatically uses
`qcint <https://github.com/sunqm/qcint>`_, a high-performance SIMD-optimized version of libcint.

Installation
------------

libcint is automatically compiled and bundled when you install GBasis:

.. code-block:: bash

   pip install qc-gbasis

No additional setup is required. libcint is supported on:

- **Linux** (x86_64, ARM)
- **macOS** (x86_64, Apple Silicon ARM64)

.. note::
   Windows is not currently supported due to MSVC compiler incompatibility with
   libcint's C99 VLA syntax.

Quick Start
-----------

.. code-block:: python

   import numpy as np
   from gbasis.parsers import make_contractions, parse_nwchem
   from gbasis.integrals.libcint import CBasis

   # Define molecule (H2, coordinates in Bohr)
   atsyms = ['H', 'H']
   atcoords = np.array([[0., 0., -0.7], [0., 0., 0.7]])

   # Load basis set
   basis_dict = parse_nwchem('data_sto6g.nwchem')
   py_basis = make_contractions(basis_dict, atsyms, atcoords, coord_types='spherical')

   # Create CBasis — libcint interface
   cb = CBasis(py_basis, atsyms, atcoords, coord_type='spherical')

   # Compute integrals
   S = cb.overlap_integral()           # Overlap
   T = cb.kinetic_energy_integral()    # Kinetic energy
   V = cb.nuclear_attraction_integral() # Nuclear attraction

CBasis API
----------

The ``CBasis`` class is the main interface to the libcint backend.

.. code-block:: python

   CBasis(basis, atnums, atcoords, coord_type='spherical')

**Parameters:**

- ``basis`` — List of ``GeneralizedContractionShell`` objects
- ``atnums`` — List of element symbols (e.g., ``['H', 'He']``)
- ``atcoords`` — Array of atomic coordinates in Bohr
- ``coord_type`` — ``'spherical'`` or ``'cartesian'``

Available Integrals
-------------------

1-Electron Integrals
~~~~~~~~~~~~~~~~~~~~

.. list-table::
   :header-rows: 1

   * - Method
     - Description
     - Return Shape
   * - ``overlap_integral()``
     - Overlap matrix
     - ``(nbfn, nbfn)``
   * - ``kinetic_energy_integral()``
     - Kinetic energy matrix
     - ``(nbfn, nbfn)``
   * - ``nuclear_attraction_integral()``
     - Nuclear attraction matrix
     - ``(nbfn, nbfn)``
   * - ``momentum_integral(origin)``
     - Momentum integrals
     - ``(nbfn, nbfn, 3)``
   * - ``angular_momentum_integral()``
     - Angular momentum integrals (GBasis implementation)
     - ``(nbfn, nbfn, 3)``
   * - ``moment_integral(orders, origin)``
     - Multipole moment integrals
     - ``(nbfn, nbfn, N)``
   * - ``r_inv_integral(origin)``
     - 1/r operator integrals
     - ``(nbfn, nbfn)``
   * - ``point_charge_integral(coords, charges)``
     - Point charge integrals
     - ``(nbfn, nbfn, N)``

2-Electron Integrals
~~~~~~~~~~~~~~~~~~~~

.. list-table::
   :header-rows: 1

   * - Method
     - Description
     - Return Shape
   * - ``electron_repulsion_integral()``
     - Electron repulsion integrals (ERI)
     - ``(nbfn, nbfn, nbfn, nbfn)``

Gradient Integrals
~~~~~~~~~~~~~~~~~~

.. list-table::
   :header-rows: 1

   * - Method
     - Description
     - Return Shape
   * - ``overlap_gradient_integral()``
     - Overlap gradient
     - ``(nbfn, nbfn, 3)``
   * - ``kinetic_gradient_integral()``
     - Kinetic energy gradient
     - ``(nbfn, nbfn, 3)``
   * - ``nuclear_gradient_integral()``
     - Nuclear attraction gradient
     - ``(nbfn, nbfn, 3)``
   * - ``electron_repulsion_gradient_integral()``
     - ERI gradient
     - ``(nbfn, nbfn, nbfn, nbfn, 3)``
   * - ``r_inv_gradient_integral(origin)``
     - 1/r gradient
     - ``(nbfn, nbfn, 3)``

3-Center Integrals
~~~~~~~~~~~~~~~~~~

.. list-table::
   :header-rows: 1

   * - Method
     - Description
     - Return Shape
   * - ``three_center_repulsion_integral()``
     - 3-center 2-electron repulsion
     - ``(nbfn, nbfn, nbfn)``

Platform Detection
------------------

GBasis automatically detects the hardware platform and selects the optimal library:

- **x86_64 + AVX2**: Uses ``qcint`` (SIMD-optimized)
- **ARM / non-AVX**: Uses ``libcint`` (generic)

This is done at compile time via CMake:

.. code-block:: cmake

   if(HAS_AVX2 AND NOT CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
       # Use qcint — x86 + AVX2 optimized
   else()
       # Use libcint — generic
   endif()

References
----------

- libcint paper: Qiming Sun, *J. Comp. Chem.*, 2015, 36, 1664
- libcint v6 paper: Qiming Sun, *J. Chem. Phys.*, 2024
- `libcint GitHub <https://github.com/sunqm/libcint>`_
- `qcint GitHub <https://github.com/sunqm/qcint>`_
- `GBasis Issue #229 <https://github.com/theochem/gbasis/issues/229>`_
