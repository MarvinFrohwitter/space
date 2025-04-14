#ifndef SPACE_H_
#define SPACE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Planet Planet;
struct Planet {
  void *elements;
  size_t count;
  size_t capacity;
  size_t id;

  Planet *prev;
  Planet *next;
};

typedef struct {
  Planet *sun;
  size_t planet_count;
} Space;

Planet *space_init_planet(Space *space, size_t size_in_bytes);
void space_free_planet(Space *space, Planet *planet);
void space_free_space(Space *space);
void space_reset_planet(Planet *planet);
bool space_reset_planet_id(Space *space, size_t id);
void space_reset_space(Space *space);
void *space_malloc(Space *space, size_t size_in_bytes);
void *space_calloc(Space *space, size_t nmemb, size_t size);
void *space_realloc(Space *space, void *ptr, size_t old_size, size_t new_size);
bool space_init_capacity(Space *space, size_t size_in_bytes);

#endif // SPACE_H_

// ===========================================================================

#ifdef SPACE_IMPLEMENTATION

Planet *space_init_planet(Space *space, size_t size_in_bytes) {
  Planet *planet = malloc(sizeof(*planet));
  if (planet) {
    memset(planet, 0, sizeof(*planet));
    planet->capacity = size_in_bytes;
    planet->count = size_in_bytes;
    planet->elements = malloc(planet->capacity);
    if (!planet->elements) {
      free(planet);
      return NULL;
    }
    planet->id = space->planet_count++;
  }

  return planet;
}

void space_free_planet(Space *space, Planet *planet) {
  if (!planet) {
    return;
  }
  if (planet->prev) {
    planet->prev->next = planet->next;
  }
  if (planet->next) {
    planet->next->prev = planet->prev;
  }

  free(planet->elements);
  planet->elements = NULL;
  planet->count = 0;
  planet->capacity = 0;
  planet->next = NULL;
  planet->prev = NULL;
  planet->id = 0;
  free(planet);
  planet = NULL;

  space->planet_count--;
}

void space_free_space(Space *space) {
  for (Planet *next, *planet = space->sun; planet; planet = next) {
    next = planet->next;
    space_free_planet(space, planet);
  }
}

void space_reset_planet(Planet *planet) { planet->count = 0; }

bool space_reset_planet_id(Space *space, size_t id) {
  for (Planet *planet = space->sun; planet; planet = planet->next) {
    if (planet->id == id) {
      space_reset_planet(planet);
      return true;
    }
  }
  return false;
}

void space_reset_space(Space *space) {
  for (Planet *p = space->sun; p; p = p->next) {
    space_reset_planet(p);
  }
}

void *space_malloc(Space *space, size_t size_in_bytes) {
  if (!space) {
    return NULL;
  }

  if (!space->planet_count) {
    space->sun = space_init_planet(space, size_in_bytes);
    if (!space->sun) {
      return NULL;
    }
    space->sun->next = NULL;
    space->sun->prev = NULL;
    return space->sun->elements;
  }

  Planet *p = space->sun;
  Planet *prev = space->sun;
  for (; p; prev = p, p = p->next) {
    if (p->count + size_in_bytes > p->capacity) {
      continue;
    }

    void *place = &((uintptr_t *)p->elements)[p->count];
    p->count += size_in_bytes;
    return place;
  }

  p = space_init_planet(space, size_in_bytes);
  if (!p) {
    return NULL;
  }

  p->prev = prev;
  p->next = NULL;
  prev->next = p;
  return prev->next->elements;
}

void *space_calloc(Space *space, size_t nmemb, size_t size) {
  size_t size_in_bytes = nmemb * size;
  void *ptr = space_malloc(space, size_in_bytes);
  memset(ptr, 0, size_in_bytes);
  return ptr;
}

void *space_realloc(Space *space, void *ptr, size_t old_size, size_t new_size) {
  if (old_size >= new_size) {
    return ptr;
  }
  char *new_ptr = space_malloc(space, new_size);
  memcpy(new_ptr, ptr, old_size);
  return new_ptr;
}

bool space_init_capacity(Space *space, size_t size_in_bytes) {
  if (space_malloc(space, size_in_bytes)) {
    return true;
  }
  return false;
}

#endif // SPACE_IMPLEMENTATION
