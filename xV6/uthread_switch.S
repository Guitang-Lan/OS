    .text

/* Switch from current_thread to next_thread. Make next_thread
 * the current_thread, and set next_thread to 0.
 * Use eax as a temporary register, which should be caller saved.
 */
    .globl thread_switch
thread_switch:
    /* YOUR CODE HERE */
    pushal
    movl current_thread, %eax
    movl %esp, (%eax)
    movl next_thread, %eax
    movl (%eax), %esp
    movl %eax, current_thread
    movl $0x0, next_thread
    popal
    ret             /* pop return address from stack */