import { render, screen } from '@testing-library/react'
import { beforeAll, describe, expect, test } from 'vitest'
import App from './App'

beforeAll(() => {
  Object.defineProperty(window, 'matchMedia', {
    writable: true,
    value: query => ({
      matches: false,
      media: query,
      onchange: null,
      addListener: () => {},
      removeListener: () => {},
      addEventListener: () => {},
      removeEventListener: () => {},
      dispatchEvent: () => false
    })
  })
})

describe('App', () => {
  test('renders todo list title', () => {
    render(<App />)
    expect(screen.getByText(/todo list/i)).toBeInTheDocument()
  })
})
