#include <err.h>
#include <sysexits.h>

#include <libavcodec/opt.h>
#include <libavformat/avformat.h>

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
  av_register_all ();

  av_log_set_level (0);

  if (0 > (res = avformat_open_input (&input_formatctx, path, NULL, NULL)))
    {
      av_strerror (res, errbuf, sizeof (errbuf));

      fprintf (stderr, "Failed to open '%s': %s\n", path, errbuf);
      printf ("60\n");

      return EXIT_FAILURE;
    }

  if (0 > (res = avformat_find_stream_info (input_formatctx, NULL)))
    {
      av_strerror (res, errbuf, sizeof (errbuf));

      fprintf (stderr, "Failed to find stream information in '%s': %s\n", path, errbuf);
      printf ("60\n");

      return EXIT_FAILURE;
    }

  for (streamidx = 0; streamidx < input_formatctx->nb_streams; ++streamidx)
    {
      AVStream *stream;

      stream = input_formatctx->streams[streamidx];

      switch (stream->codec->codec_type)
        {
        case AVMEDIA_TYPE_VIDEO:

            {
              double fps;

              fps = (double) stream->r_frame_rate.num / stream->r_frame_rate.den;

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
