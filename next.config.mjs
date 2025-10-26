import nextra from 'nextra';

const withNextra = nextra({
  latex: true,
  search: {
    codeblocks: false,
  },
});

const nextOptions = {
  output: 'export',
  images: {
    unoptimized: true,
  },
};

export default withNextra(nextOptions);
