/* Numerical model regression, not an MCU/hardware test. This executes the
 * production heading.c scalar logic after adapting its small C syntax subset
 * to JavaScript. Native ARM compilation is checked separately by Keil.
 * JS numbers are doubles, so floating-point ABI/timing are not covered. */
const fs = require('fs');
const path = require('path');
const vm = require('vm');
const assert = require('assert');
let source = fs.readFileSync(path.join(__dirname, '../BSP/PID/heading.c'), 'utf8');
source = source.replace(/^#include[^\n]*$/gm, '')
 .replace(/\b(?:float|void)\s+(heading_\w+)\(([^)]*)\)/g, (_, name, args) =>
  `function ${name}(${args.split(',').map(a => a.trim().match(/([A-Za-z_]\w*)$/)[1]).join(',')})`)
 .replace(/\bfloat\s+/g, 'let ')
 .replace(/->/g, '.')
 .replace(/(\d+(?:\.\d*)?)f\b/g, '$1')
 .replace(/\bisfinite\(/g, 'Number.isFinite(');
const context = vm.createContext({});
vm.runInContext(source, context);
const {heading_begin: begin, heading_step: step, heading_error: error} = context;
assert.equal(typeof begin, 'function');
const near = (a, b) => assert.ok(Math.abs(a-b)<1e-5, `${a} != ${b}`);
const state = (yaw, valid=1) => {const s={};begin(s,yaw,valid);return s;};

near(error(179,-179),-2);near(error(-179,179),2);
near(error(30,30),0);near(error(0,360),0);
let s=state(30);near(step(s,30,0,1),0);
// A left heading deviation must request a RIGHT turn (negative differential).
s=state(0);assert.ok(step(s,5,0,1)<0);
s=state(0);assert.ok(step(s,-5,0,1)>0);
// Rate feedback opposes ongoing rotation before the angle error grows.
s=state(0);assert.ok(step(s,0,10,1)<0);
// Large error must never command a wheel reversal or exceed the slew limit.
s=state(0);let previous=0;
for(let i=0;i<300;i++){
 const c=step(s,-90,0,1);
 assert.ok(Math.abs(c)<=8 && Math.abs(c-previous)<=0.50001);
 assert.ok(30+c>=22 && 30+c<=38 && 30-c>=22 && 30-c<=38);
 near(s.target,0);previous=c;
}
near(s.correction,8);
// Stale data clears steering immediately, without moving the frozen target.
near(step(s,-90,0,0),0);near(s.target,0);
assert.ok(step(s,-5,0,1)>0);near(s.target,0);
// An invalid exit sample must not silently lock a different heading later.
s=state(40,0);near(step(s,60,0,1),0);near(s.target,40);
// A NEW line exit may capture a new target and has no prior differential.
begin(s,60,1);near(s.target,60);near(s.correction,0);
near(step(s,60,0,1),0);
s=state(0);near(step(s,0.2,0,1),0);
near(step(s,NaN,0,1),0);near(step(s,0,Infinity,1),0);
s=state(NaN);assert.equal(s.locked,0);
console.log('PASS: wrap, direction, damping, limits, target freeze, stale data, reacquisition, deadband.');
