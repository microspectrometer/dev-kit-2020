test/test_runner.c:126:test_QueueInit_sets_buffer_length:PASS
test/test_runner.c:127:test_QueueLength_is_0_after_QueueInit:PASS
test/test_runner.c:128:test_QueueLength_increments_after_a_push:PASS
test/test_runner.c:129:test_QueuePush_writes_to_byte_pointed_to_by_head:PASS
test/test_runner.c:130:test_QueuePush_increments_head:PASS
test/test_runner.c:131:test_QueuePush_does_nothing_if_Queue_is_full:PASS
test/test_runner.c:132:test_QueueLength_does_not_increase_beyond_max_length:PASS
test/test_runner.c:133:test_QueueIsFull_returns_true_if_Queue_is_full:PASS
test/test_runner.c:134:test_QueueIsFull_returns_false_if_Queue_is_not_full:PASS
test/test_runner.c:135:test_QueueLength_decrements_after_a_pop:PASS
test/test_runner.c:136:test_QueuePop_reads_byte_pointed_to_by_tail:PASS
test/test_runner.c:137:test_QueuePop_increments_tail:PASS
test/test_runner.c:138:test_QueueLength_does_not_decrease_below_zero:PASS
test/test_runner.c:139:test_QueuePop_returns_0_if_Queue_is_empty:PASS
test/test_runner.c:140:test_QueuePop_does_not_increment_tail_if_Queue_is_empty:PASS
test/test_runner.c:141:test_QueueIsEmpty_returns_true_if_Queue_is_empty:PASS
test/test_runner.c:142:test_QueueIsEmpty_returns_false_if_Queue_is_not_empty:PASS
test/test_runner.c:143:test_QueuePush_wraps_head_back_to_buffer_index_0:PASS
test/test_runner.c:144:test_QueuePop_wraps_tail_back_to_buffer_index_0:PASS

-----------------------
19 Tests 0 Failures 0 Ignored 
OK
