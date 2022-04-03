#ifndef OPENPYROJET_STATE_MACHINE_H
#define OPENPYROJET_STATE_MACHINE_H

/***
 * An action for a screen.
 * The function argument "void*" is the state of the screen.
 */
typedef void (*state_action)();

struct StateMachine;

struct StateMachine* state_machine_create(const char* name, int stateCount);

void state_machine_delete(struct StateMachine* stateMachine);

/**
 * @param machine the state machine
 * @param onEnter (nullable) called when the state is activated
 * @param onUpdate (nullable) called when the state data has changed
 * @param onExit (nullable) called when the state is deactivated
 * @param data (nullable) state data
 * @return the screen id
 */
int state_machine_add_state(struct StateMachine* machine, const char* name, state_action onEnter, state_action onUpdate, state_action onExit);

/**
 * Transition to a new state. Deactivates the previous state if applicable.
 * @param machine the state machine
 * @param id the state id
 */
void state_machine_enter(struct StateMachine* machine, int id);

/**
 * If the specified state is active: send an update signal to trigger the update callback.
 * @param machine the state machine
 * @param id the screen id
 */
void state_machine_update(struct StateMachine* machine, int id);


#endif //OPENPYROJET_STATE_MACHINE_H
