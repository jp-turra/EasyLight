import Vue from 'vue'
import Vuex from 'vuex'
import dados from './dados'
import createPersistedState from 'vuex-persistedstate'

Vue.use(Vuex)

/*
 * If not building with SSR mode, you can
 * directly export the Store instantiation
 */

export default function (/* { ssrContext } */) {
  const Store = new Vuex.Store({
    modules: {
      dados
    },
    plugins: [createPersistedState({ path: ['device', 'userName'] })],
    // enable strict mode (adds overhead!)
    // for dev mode only
    strict: process.env.DEBUGGING
  })
  return Store
}
