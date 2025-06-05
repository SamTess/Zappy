import {themes as prismThemes} from 'prism-react-renderer';
import type {Config} from '@docusaurus/types';
import type * as Preset from '@docusaurus/preset-classic';

// This runs in Node.js - Don't use client-side code here (browser APIs, JSX...)

const config: Config = {
  title: 'Zappy',
  tagline: 'Un serveur de jeu multijoueur avec client GUI et bot IA',
  favicon: 'img/favicon.ico',

  // Set the production url of your site here
  url: 'https://epitech.github.io',
  // Set the /<baseUrl>/ pathname under which your site is served
  // For GitHub pages deployment, it is often '/<projectName>/'
  baseUrl: '/Zappy/',

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: 'Epitech', // Usually your GitHub org/user name.
  projectName: 'Zappy', // Usually your repo name.

  onBrokenLinks: 'throw',
  onBrokenMarkdownLinks: 'warn',

  // Even if you don't use internationalization, you can use this field to set
  // useful metadata like html lang. For example, if your site is Chinese, you
  // may want to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: 'fr',
    locales: ['fr', 'en'],
  },

  presets: [
    [
      'classic',
      {
        docs: {
          sidebarPath: './sidebars.ts',
          // Please change this to your repo.
          // Remove this to remove the "edit this page" links.
          editUrl:
            'https://github.com/Epitech/Zappy/tree/main/docs/docusaurus/',
        },
        // blog section removed as it's not needed for Zappy documentation
        theme: {
          customCss: './src/css/custom.css',
        },
      } satisfies Preset.Options,
    ],
  ],

  themeConfig: {
    // Replace with your project's social card
    image: 'img/zappy-social-card.jpg',
    navbar: {
      title: 'Zappy',
      logo: {
        alt: 'Zappy Logo',
        src: 'img/logo.svg',
      },
      items: [
        {
          type: 'docSidebar',
          sidebarId: 'tutorialSidebar',
          position: 'left',
          label: 'Documentation',
        },
        {
          type: 'docSidebar',
          sidebarId: 'tutorialSidebar',
          position: 'left',
          label: 'Serveur',
          to: '/docs/server',
        },
        {
          type: 'docSidebar',
          sidebarId: 'tutorialSidebar', 
          position: 'left',
          label: 'GUI',
          to: '/docs/gui',
        },
        {
          type: 'docSidebar',
          sidebarId: 'tutorialSidebar',
          position: 'left',
          label: 'AI',
          to: '/docs/ai',
        },

        {
          type: 'localeDropdown',
          position: 'right',
        },
        {
          href: 'https://github.com/Epitech/Zappy',
          label: 'GitHub',
          position: 'right',
        },
      ],
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Documentation',
          items: [
            {
              label: 'Introduction',
              to: '/docs/intro',
            },
            {
              label: 'Guide de démarrage',
              to: '/docs/getting-started',
            },
            {
              label: 'Architecture',
              to: '/docs/architecture',
            },
          ],
        },
        {
          title: 'Composants',
          items: [
            {
              label: 'Serveur',
              to: '/docs/server',
            },
            {
              label: 'Interface Graphique',
              to: '/docs/gui',
            },
            {
              label: 'Intelligence Artificielle',
              to: '/docs/ai',
            },
          ],
        },
        {
          title: 'Liens',
          items: [

            {
              label: 'GitHub',
              href: 'https://github.com/Epitech/Zappy',
            },
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} Epitech, Projet Zappy. Construit avec Docusaurus.`,
    },
    prism: {
      theme: prismThemes.github,
      darkTheme: prismThemes.dracula,
    },
  } satisfies Preset.ThemeConfig,
};

export default config;
