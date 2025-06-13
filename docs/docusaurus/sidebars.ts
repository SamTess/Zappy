import type {SidebarsConfig} from '@docusaurus/plugin-content-docs';

/**
 * Creating a sidebar enables you to:
 - create an ordered group of docs
 - render a sidebar for each doc of that group
 - provide next/previous navigation

 The sidebars can be generated from the filesystem, or explicitly defined here.

 Create as many sidebars as you want.
 */
const sidebars: SidebarsConfig = {
  // Documentation principale de Zappy avec structure manuelle
  tutorialSidebar: [
    'intro',
    'getting-started',
    'architecture',
    'development-guide',
    {
      type: 'category',
      label: 'Serveur',
      items: [
        'server/server-overview',
      ],
    },
    {
      type: 'category',
      label: 'Interface Graphique',
      items: [
        'gui/gui-overview',
      ],
    },
    {
      type: 'category',
      label: 'Intelligence Artificielle',
      items: [
        'ai/ai-overview',
      ],
    },
    {
      type: 'category',
      label: 'Protocoles',
      items: [
        'protocols/server-ai',
        'protocols/server-gui',
      ],
    },
    {
      type: 'category',
      label: 'Tutoriels de Base',
      items: [
        'tutorial-basics/create-a-document',
        'tutorial-basics/create-a-page',
        'tutorial-basics/create-a-blog-post',
        'tutorial-basics/markdown-features',
        'tutorial-basics/deploy-your-site',
        'tutorial-basics/congratulations',
      ],
    },
    {
      type: 'category',
      label: 'Fonctionnalités Avancées',
      items: [
        'tutorial-extras/manage-docs-versions',
        'tutorial-extras/translate-your-site',
      ],
    },
  ],
};

export default sidebars;
