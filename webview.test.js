import { hello } from './webview.js'

test('webview', () => {
  const result = hello()
  expect(result).toBe('world')
})
