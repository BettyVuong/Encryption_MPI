# String Decryption with Task Parallelism
This project explores a bruteforce decryption algorithm using a serial and parallel implementation using MPI's. First, the string is encrypted, then the decryption algorithm is used.

## Takeaways
- Learning about MPI's using OpenMPI and how this could be scaleable for distributed systems
  - Specifically Butterfly all reduce algorithm
- Task parallelism use-cases
- Decryption and encryption algorithms
- Analyzing data from expiramental programs
- Understanding factors affecting performance (OS, CPU activity, hardware)

## How to Run
1. "make" the files
2. Encrypt the string using ```./a2encrypt <insert string>```
3. To decrypt with parallelism  ```mpiexec -n N ./a2decrypt ciphertext.txt <insert deference dictionary file name>```, where N = # of MPI processes
4. To decrypt with serial ```./a2decrypt_serial ciphertext.txt <insert deference dictionary file name>```