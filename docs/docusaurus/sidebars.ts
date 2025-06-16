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
      label: 'Server',
      items: [
        'server/server-overview',
      ],
    },
    {
      type: 'category',
      label: 'GUI',
      items: [
        'gui/gui-overview',
      ],
    },
    {
      type: 'category',
      label: 'AI',
      items: [
        'ai/ai-overview',
      ],
    },
    {
      type: 'category',
      label: 'Protocols',
      items: [
        'protocols/server-ai',
        'protocols/server-gui',
      ],
    },
  ],
};

export default sidebars;
