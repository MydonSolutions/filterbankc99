# Filterbank file library

Supports both the SIGPROC and HDF5 (FBH5) formats.

Enables writing the data portion from a buffer order by [Channel=slowest, Time, Polarization] (while trying to be performant about it).

Format | Capabilities | API
-|-|-
SIGPROC | Read and Write | Character Buffer Header, File Descriptor and Struct
HDF5 (FBH5) | Write | Struct

See [tests/filterbank.c](./tests/filterbank.c) for implementation code of all use cases.

<details><summary>HDF5 helper library</summary>
This library uses the (h5dsc99)[https://github.com/MydonSolutions/fbh5c99] library to abstract the HDF5 operations.
</details>


<details><summary>SIGPROC implementation Origin</summary>
This library was initially drawn from rawspec code:

Origin of code here is:
- [include/filterbankc99.h](https://github.com/UCBerkeleySETI/rawspec/blob/188981a992eb6fddd105dee0d7763b07802fc861/rawspec_fbutils.h)
- [include/filterbankc99.h](https://github.com/UCBerkeleySETI/rawspec/blob/188981a992eb6fddd105dee0d7763b07802fc861/rawspec_callback.h#L8-L21)
- [src/fb.c](https://github.com/UCBerkeleySETI/rawspec/blob/188981a992eb6fddd105dee0d7763b07802fc861/rawspec_fbutils.c)
- [tests/filterbank.c](https://github.com/UCBerkeleySETI/rawspec/blob/188981a992eb6fddd105dee0d7763b07802fc861/rawspec_fbutils.c#L577-L668)

</details>

<details><summary>FBH5 implementation Origin</summary>
This library consumes and expanded upon the [fbh5c99](https://github.com/MydonSolutions/fbh5c99) repository. It replaced the [filterbankh5c99](https://github.com/MydonSolutions/filterbankh5c99) repository which also drew from rawspec's sources, but had an undiagnosable error when deployed in a hashpipe thread which was its primary use case.
</details>
