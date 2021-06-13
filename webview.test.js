import { createWebview } from './webview.js'

test('webview', () => {
  const result = createWebview()
  expect(result).toBe('world')
})
