/*
 *  Copyright (C) 2011 Universidad Simon Bolivar
 * 
 *  Permission is hereby granted to distribute this software for
 *  non-commercial research purposes, provided that this copyright
 *  notice is included with any such distribution.
 *  
 *  THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 *  EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
 *  SOFTWARE IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU
 *  ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
 *  
 *  Blai Bonet, bonet@ldc.usb.ve
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include <cassert>
#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>

namespace Utils {

  // Cronometro.
  void cronometer(int seconds, pid_t parent) {
    sleep(seconds);
    kill(parent, SIGINT);
    exit(0);
  }

  /*
    Para poder parar la ejecucion.
  */
  void set_handler(void (*f)(int)) {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = f;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
  }

  template <typename D>
  void print_vector(vector<D> V) {
    cout << "[";
    for (D d : V) cout << d << ", ";
    cout << "]\n";
  }
} 

#endif

