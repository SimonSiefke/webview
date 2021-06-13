import bindings from 'bindings'

const addon = bindings('webview')

export const hello = () => addon.hello()
