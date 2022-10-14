# Boost Beast Service (OSx)

This repository was put together to make triaging issues within Boost </br>
Beast on Mac OSX. At the time of creation (Fri October 14, 2022) this </br>
was done on a MacBook Pro (2019), 2.6 GHz 6-Core Intel Core i7 Machine </br>
running macOs Monterey Version 12.6. At time reading in the request body </br>
would halt the server after making a GET request on localhost. </br>

## Build

To build for the first time:

```sh
make deps all
```

## Run

To run:

```sh
./build/https_service/bin/https_service
```
