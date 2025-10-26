/* eslint-env node */
import '@/src/styles/globals.css';
import { Footer, Layout, Navbar } from 'nextra-theme-docs'
import { Banner, Head } from 'nextra/components'
import { getPageMap } from 'nextra/page-map'
import { Logo } from '@/src/components/Logo'
import 'nextra-theme-docs/style.css'

export const metadata = {
  metadataBase: new URL('https://docs.webserial.pro'),
  title: {
    template: '%s - WebSerial Docs'
  },
  description: 'WebSerial: A remote terminal to log, monitor or debug your embedded devices over a wireless connection.',
  applicationName: 'WebSerial Docs',
  generator: 'Next.js',
  appleWebApp: {
    title: 'WebSerial Docs'
  }
}

export default async function RootLayout({ children }) {
  const navbar = (
    <Navbar
      logo={<Logo />}
      projectLink="https://github.com/ayushsharma82/WebSerial"
    >
      <a href="https://webserial.pro" target="_blank" className="text-zinc-900 bg-white hover:bg-zinc-100 border border-zinc-200 focus:ring-4 focus:outline-hidden focus:ring-zinc-100 font-medium rounded-lg text-sm px-5 py-2 text-center inline-flex items-center dark:focus:ring-zinc-600 dark:bg-zinc-800 dark:border-zinc-700 dark:text-white dark:hover:bg-zinc-700">
        <span className='hidden sm:block me-1'>
          Upgrade to
        </span>
        <img src="/assets/images/pro-badge.gif" alt="" width={38} />
        <svg xmlns="http://www.w3.org/2000/svg" width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round" className="ms-2"><path d="M5 12h14" /><path d="m12 5 7 7-7 7" /></svg>
      </a>
    </Navbar>
  )
  
  const pageMap = await getPageMap()
  
  return (
    <html lang="en" dir="ltr" suppressHydrationWarning>
      <Head faviconGlyph="✦" color={{ hue: { dark: 156, light: 156 }, saturation: { dark: 86, light: 58 }, lightness: { dark: 46, light: 37 } }} />
      <body>
        <Layout
          // banner={<Banner storageKey="blackfriday-2025">Black Friday 2025 is Live!</Banner>}
          navbar={navbar}
          footer={
            <div className="flex flex-col items-center text-xs py-4 px-4 text-zinc-500">
              <div>Copyright © { new Date().getFullYear() } <a href="https://softt.io" target="_blank">Softt</a>. All rights reserved.</div>
            </div>
          }
          editLink="Edit this page on GitHub"
          docsRepositoryBase="https://github.com/ayushsharma82/WebSerial/blob/docs/"
          sidebar={{ defaultMenuCollapseLevel: 2 }}
          pageMap={pageMap}
        >
          {children}
        </Layout>
      </body>
    </html>
  )
}