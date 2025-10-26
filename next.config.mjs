import nextra from 'nextra';

const withNextra = nextra({
  latex: true,
  search: {
    codeblocks: false,
  },
  contentDirBasePath: '/',
});

const nextOptions = {
  reactStrictMode: true,
  output: 'export',
  images: {
    unoptimized: true,
  },
};

export default withNextra(nextOptions);
