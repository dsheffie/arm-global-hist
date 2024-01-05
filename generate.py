#!/usr/bin/python3

#crappy implementation of section 3.3 of https://cseweb.ucsd.edu/~tullsen/halfandhalf.pdf

def gen_code(n,osx,o,ll):
    l = ''
    takenfiller = True
    if osx:
        l = '_'
        o.write('.globl _foo%d\n' % n)
    else:
        o.write('.global foo%d\n' % n)

    if takenfiller:
        bt = 'z'
    else:
        bt = 'nz'
        
    o.write('%sfoo%d:\n' % (l, n))
    o.write('eor	w3, w3, w3\n')    
    o.write('.L%d:\n' % ll)
    ss = ll
    ll = ll + 1
    o.write('eor	w0, w0, w0, lsl 13\n')
    o.write('eor	w2, w0, w0, lsr 17\n')
    o.write('eor	w0, w2, w2, lsl 5\n')
    o.write('tbz	x2, 0, .L%d\n' % ll)
    o.write('nop\n')
    o.write('.L%d:\n' % ll)
    ll = ll + 1
    for i in range(0, n):
        o.write('tb%s w3, 0, .LL%d\n' % (bt, ll))
        o.write('.LL%d:\n' % ll)
        ll = ll + 1
    o.write('tbz	x2, 0, .L%d\n' % ll)
    o.write('nop\n')
    o.write('.L%d:\n' % ll)
    ll = ll + 1
    o.write('subs	w1, w1, #1\n')
    o.write('bne	.L%d\n' % ss)
    o.write('ret\n')
    return ll


if __name__ == '__main__':
    o = open('functions.s', 'w')
    ll = 0
    n = 1024
    for i in range(1, n):
        ll = gen_code(i, True, o, ll)
    o.close()
    o = open('header.h', 'w')
    o.write('#include <stdint.h>\n')
    for i in range(1, n):
        o.write('uint32_t foo%d(uint32_t x, uint32_t c);\n' % i)

    o.write('uint32_t (*funcs[]) (uint32_t, uint32_t) = {\n')
    for i in range(1, n):
        o.write('foo%d,\n' % i)
    
    o.write('};\n')
        
    o.close()
        
