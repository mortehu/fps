#include <err.h>
#include <sysexits.h>
#include <libavformat/avformat.h>
#include <libavcodec/opt.h>

int
main (int argc, char **argv)
{
  const char *path;
  AVFormatContext*   input_formatctx = 0;

  int streamidx;
  char errbuf[256];
  int res;

  if (argc != 2)
    errx (EX_USAGE, "Usage: %s <PATH>", argv[0]);

  path = argv[1];
  av_register_all();

  av_log_set_level (0);

  if(0 > (res = av_open_input_file(&input_formatctx, path, 0, 0, 0)))
    {
      fprintf(stderr, "Failed to open '%s': %s\n", path, av_strerror(res, errbuf, sizeof (errbuf)));
      printf ("60\n");

      return EXIT_FAILURE;
    }

  if(0 > (res = av_find_stream_info(input_formatctx)))
    {
      printf("Failed to find stream information in '%s': %s\n", path, av_strerror(res, errbuf, sizeof (errbuf)));
      printf ("60\n");

      return EXIT_FAILURE;
    }

  for(streamidx = 0; streamidx < input_formatctx->nb_streams; ++streamidx)
    {
      switch(input_formatctx->streams[streamidx]->codec->codec_type)
        {
        case AVMEDIA_TYPE_VIDEO:

            {
              double fps;

              fps = (double) input_formatctx->streams[streamidx]->r_frame_rate.num / input_formatctx->streams[streamidx]->r_frame_rate.den;

              printf ("%.f\n", ceil (fps));

              return EXIT_SUCCESS;
            }

          break;

        default:;
        }
    }

  printf ("60\n");

  return EXIT_SUCCESS;
}
