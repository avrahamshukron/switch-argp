# switch-argp
CLI parsing example using GNU Argp

In this example I explore the basic usage of argp, including:
1. Positional arguments
2. Options (short and long)
3. Basic parsing and validation
4. Error handling


## CLI

This is how it looks like from the shell:
```
$ ./switch --help
Usage: switch [OPTION...] NAME ID SOCK_TYPE ADDRESS
Starts an Infected Switch

  -d, --debug                Enable debug log output
  -r, --router=PATH          The unix socket address of the Infected Router
  -s, --stream               Whether the input source is a stream (not
                             seqpacket)
      --server=PATH          The unix socket address created for clients by
                             this switch
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

  NAME         The name of this switch
  ID           The Infected DST which the switch binds to, as a 2-byte
               hex string, like 0xfa9b
  SOCK_TYPE    How the switch will connect to it's source. Valid
               options are 'tcp', 'unix' or 'dev'
  ADDRESS      The address for the infected source. The value is
               dependant on the SOCK_TYPE specified. For 'unix', 'dev'
               this should be a UNIX path. For 'tcp' this should be
               an <IP>:<PORT> string
```