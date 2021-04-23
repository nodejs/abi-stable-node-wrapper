/* Test helpers ported from test/common/index.js in Node.js project. */
'use strict';
const assert = require('assert');

const noop = () => {};

const mustCallChecks = [];

function runCallChecks(exitCode) {
  if (exitCode !== 0) return;

  const failed = mustCallChecks.filter(function(context) {
    if ('minimum' in context) {
      context.messageSegment = `at least ${context.minimum}`;
      return context.actual < context.minimum;
    } else {
      context.messageSegment = `exactly ${context.exact}`;
      return context.actual !== context.exact;
    }
  });

  failed.forEach(function(context) {
    console.log('Mismatched %s function calls. Expected %s, actual %d.',
                context.name,
                context.messageSegment,
                context.actual);
    console.log(context.stack.split('\n').slice(2).join('\n'));
  });

  if (failed.length) process.exit(1);
}

exports.mustCall = function(fn, exact) {
  return _mustCallInner(fn, exact, 'exact');
};
exports.mustCallAtLeast = function(fn, minimum) {
  return _mustCallInner(fn, minimum, 'minimum');
};

function _mustCallInner(fn, criteria, field) {
  if (typeof fn === 'number') {
    criteria = fn;
    fn = noop;
  } else if (fn === undefined) {
    fn = noop;
  }
  if (criteria === undefined) {
    criteria = 1;
  }

  if (typeof criteria !== 'number')
    throw new TypeError(`Invalid ${field} value: ${criteria}`);

  const context = {
    [field]: criteria,
    actual: 0,
    stack: (new Error()).stack,
    name: fn.name || '<anonymous>'
  };

  // add the exit listener only once to avoid listener leak warnings
  if (mustCallChecks.length === 0) process.on('exit', runCallChecks);

  mustCallChecks.push(context);

  return function() {
    context.actual++;
    return fn.apply(this, arguments);
  };
}

exports.mustNotCall = function(msg) {
  return function mustNotCall() {
    assert.fail(msg || 'function should not have been called');
  };
};

exports.runTest = async function(test, buildType) {
  buildType = buildType || process.config.target_defaults.default_configuration;

  const bindings = [
    `../build/${buildType}/binding.node`,
    `../build/${buildType}/binding_noexcept.node`,
  ].map(it => require.resolve(it));

  // TODO(legendecas): investigate strange CHECK failures in Node.js core
  // - src/callback.cc
  //   - InternalCallbackScope::Close
  //     - CHECK_EQ(env_->execution_async_id(), 0);
  //
  // for (const item of bindings) {
  //   await test(require(item));
  // }
  return bindings.map(item => test(require(item)));
}

exports.runTestWithBindingPath = async function(test, buildType) {
  buildType = buildType || process.config.target_defaults.default_configuration;

  const bindings = [
    `../build/${buildType}/binding.node`,
    `../build/${buildType}/binding_noexcept.node`,
  ].map(it => require.resolve(it));

  for (const item of bindings) {
    await test(item);
  }
}
