const state = {
  page: 'inicio'
}

var mutations = {
  setPage (state, payload) {
    if (payload) state.page = payload
    else alert('Erro ao definir página')
  }
}

const getters = {
  getPage (state) {
    return state.page
  }
}

const actions = {
  setAtcion (context, payload) {
    const action = payload[0]
    Array.shift(payload)
    context.commit(action, payload)
  }
}

export default {
  namespaced: false,
  getters,
  mutations,
  actions,
  state
}
