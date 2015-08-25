{
  'targets': [
    {
      'target_name': 'foo'
      , 'sources': [ 'addon.cc', 'foo.cc' ]
      , 'include_dirs': [
          "<!(node -e \"require('nan')\")"
        , "<!(node -e \"require('..')\")"
      ]
      , 'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS': [
                '-mmacosx-version-min=10.7'
              , '-std=c++11'
              , '-stdlib=libc++'
            ]
          }
        }]
      ]
    }
  ]
}
