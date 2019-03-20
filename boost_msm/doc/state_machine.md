# State
	* data
	* entry/exit behavior (actions)
	* internal transition
		* does not call entry/exit behavior
	* deferred event
		* event not processed if state active
		
# Transition
	* the switching between active state triggered by an event
	* action and guard condition can be attached to transition
	
# Action
	* executes when transition fires
	
# Guard
	* boolean operation
	* can prevent transition from firing

