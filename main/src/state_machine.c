#include "state_machine.h"

#include <esp_log.h>

#include "types.h"

typedef struct {
    const char* name;
    state_action onEnter;
    state_action onUpdate;
    state_action onExit;
} State;

static const char* TAG = "state-machine";

struct StateMachine {
    const char* name;
    int registrationIndex;
    int activeState;
    int stateCount;
    State* states;
};

static void state_machine_no_action() {
    // NO-OP
}

StateMachine* state_machine_create(const char* name, int stateCount) {
    StateMachine* machine = malloc_one(struct StateMachine);
    machine->name = name;
    machine->stateCount = stateCount;
    machine->registrationIndex = 0;
    machine->activeState = -1;
    machine->states = malloc_array(State, stateCount);
    return machine;
}

void state_machine_delete(StateMachine* machine) {
    free(machine->states);
    free(machine);
}

int state_machine_add_state(StateMachine* machine, const char* name, state_action onEnter, state_action onUpdate, state_action onExit) {
    if (machine->registrationIndex == machine->stateCount) {
        ESP_LOGE(TAG, "[%s] reached state limit of %d", machine->name, machine->stateCount);
        return -1;
    }

    ESP_LOGI(TAG, "[%s] add %s (id %d)", machine->name, name, machine->registrationIndex);

    int id = machine->registrationIndex;
    State* state = &machine->states[id];
    state->name = name;

    if (onEnter != NULL) {
        state->onEnter = onEnter;
    } else {
        state->onEnter = &state_machine_no_action;
    }

    if (onUpdate != NULL) {
        state->onUpdate = onUpdate;
    } else {
        state->onUpdate = &state_machine_no_action;
    }

    if (onExit != NULL) {
        state->onExit = onExit;
    } else {
        state->onExit = &state_machine_no_action;
    }

    machine->registrationIndex++;
    return id;
}

void state_machine_enter(StateMachine* machine, int id) {
    if (id == machine->activeState) {
        // Already in state, so we just notify it to update
        State* state = &machine->states[id];
        ESP_LOGI(TAG, "[%s] onUpdate %s", machine->name, state->name);
        state->onUpdate();
    } else {
        // Exit the last state
        int activeState = machine->activeState;
        if (activeState != -1) {
            State* oldState = &machine->states[activeState];
            ESP_LOGI(TAG, "[%s] onExit %s", machine->name, oldState->name);
            oldState->onExit();
        }

        // Enter new state
        machine->activeState = id;
        State* state = &machine->states[id];
        ESP_LOGI(TAG, "[%s] onEnter %s", machine->name, state->name);
        state->onEnter();
        ESP_LOGI(TAG, "[%s] onUpdate %s", machine->name, state->name);
        state->onUpdate();
    }
}

void state_machine_update(StateMachine* machine, int id) {
    if (machine->activeState == id) {
        State* state = &machine->states[id];
        ESP_LOGI(TAG, "[%s] onUpdate %s", machine->name, state->name);
        state->onUpdate();
    }
}
