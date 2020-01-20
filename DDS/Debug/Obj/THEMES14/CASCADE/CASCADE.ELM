  };
  $(window).bind('hashchange', highlight_hash);
  highlight_hash();
});
</xsl:text>
</xsl:template>


<!--**==========================================================================
html.js.ui
Output JavaScript for UI extensions.
:Revision: version="1.0" date="2011-06-17" status="final"
$node: The node to create JavaScript for.

This template outputs JavaScript that implements certain common UI extensions,
such as expandable blocks and sections.
-->
<xsl:template name="html.js.ui">
  <xsl:param name="node" select="."/>
<xsl:text><![CDATA[
$.fn.yelp_ui_expander_toggle = function (onlyopen, callback) {
  var expander = $(this);
  var region = expander.children('.inner').children('.region');
  var yelpdata = expander.children('div.yelp-data-ui-expander');
  var compfunc = function () {
    if (expander.is('div.figure')) { expander.yelp_auto_resize(); }
    if (callback) { callback(); }
    return true;
  };
  var title = expander.children('.inner').children('.title');
  if (title.length == 0)
    title = expander.children('.inner').children('.hgroup');
  var title = title.find('span.title:first');
  if (expander.is('.ui-expander-e')) {
    if (!onlyopen) {
      expander.removeClass('ui-expander-e').addClass('ui-expander-c');
      region.attr('aria-expanded', 'false').slideUp('fast', compfunc);
      title.html(yelpdata.children('div.yelp-title-collapsed').html());
    }
  }
  else {
    expander.removeClass('ui-expander-c').addClass('ui-expander-e');
    region.attr('aria-expanded', 'true').slideDown('fast', compfunc);
    title.html(yelpdata.children('div.yelp-title-expanded').html());
  }
};
$(document).ready(function () {
  $('.ui-expander').each(function () {
    var expander = $(this);
    var yelpdata = expander.children('div.yelp-data-ui-expander');
    var region = expander.children('.inner').children('.region');
    var title = expander.children('.inner').children('.title');
    var issect = false;
    if (title.length == 0) {
      title = expander.children('.inner').children('.hgroup');
      issect = true;
    }
    if (title.length == 0) {
      return;
    }
    if (region.attr('id') == '')
      region.attr('id', yelp_generate_id());
    title.attr('aria-controls', region.attr('id'));
    var titlespan = title.find('span.title:first');
    var title_e = yelpdata.children('div.yelp-title-expanded');
    var title_c = yelpdata.children('div.yelp-title-collapsed');
    if (title_e.length == 0)
      yelpdata.append($('<div class="yelp-title-expanded"></div>').html(titlespan.html()));
    if (title_c.length == 0)
      yelpdata.append($('<div class="yelp-title-collapsed"></div>').html(titlespan.html()));
    if (yelpdata.attr('data-yelp-expanded') == 'false') {
      expander.addClass('ui-expander-c');
      region.attr('aria-expanded', 'false').hide();
      if (title_c.length != 0)
        titlespan.html(title_c.html());
    } else {
      expander.addClass('ui-expander-e');
      region.attr('aria-expanded', 'true');
      if (title_e.length != 0)
        titlespan.html(title_e.html());
    }
    title.click(function () {
      expander.yelp_ui_expander_toggle(false);
    });
  });
});
$(document).ready(function () {
  var expand_hash = function () {
    if (location.hash != '') {
      var target = $(location.hash);
      var parents = target.parents('div.ui-expander');
      if (target.is('div.ui-expander'))
        parents = parents.andSelf();
      parents.each(function () {
        $(this).yelp_ui_expander_toggle(true, function () {
          window.scrollTo(0, $(target).offset().top);
        });
      });
    }
  };
  $(window).bind('hashchange', expand_hash);
  expand_hash();
});
]]></xsl:text>
</xsl:template>


<!--**==========================================================================
html.js.media
Output JavaScript to control media elements.
:Revision: version="1.0" date="2010-01-01" status="final"
$node: The node to create JavaScript for.

This template outputs JavaScript that controls media elements. It provides
control for audio and video elements as well as support for captions.
-->
<xsl:template name="html.js.media">
  <xsl:param name="node" select="."/>
<xsl:text><![CDATA[
yelp_color_text_light = ']]></xsl:text>
<xsl:value-of select="$color.text_light"/><xsl:text><![CDATA[';
yelp_color_gray_background = ']]></xsl:text>
<xsl:value-of select="$color.gray_background"/><xsl:text><![CDATA[';
yelp_color_gray_border = ']]></xsl:text>
<xsl:value-of select="$color.gray_border"/><xsl:text><![CDATA[';
yelp_paint_zoom = function (zoom, zoomed) {
  var ctxt = zoom.children('canvas')[0].getContext('2d');
  ctxt.strokeStyle = ctxt.fillStyle = yelp_color_text_light;
  ctxt.clearRect(0, 0, 10, 10);
  ctxt.strokeRect(0.5, 0.5, 9, 9);
  if (zoomed) {
    ctxt.fillRect(1, 1, 9, 4);
    ctxt.fillRect(5, 5, 4, 4);
    zoom.attr('title', zoom.attr('data-zoom-out-title'));
  }
  else {
    ctxt.fillRect(1, 5, 4, 4);
    zoom.attr('title', zoom.attr('data-zoom-in-title'));
  }
}
$.fn.yelp_auto_resize = function () {
  var fig = $(this);
  if (fig.is('img'))
    fig = fig.parents('div.figure').eq(0);
  if (fig.data('yelp-zoom-timeout') != undefined) {
    clearInterval(fig.data('yelp-zoom-timeout'));
    fig.removeData('yelp-zoom-timeout');
  }
  var imgs = fig.find('img');
  for (var i = 0; i < imgs.length; i++) {
    var img = $(imgs[i]);
    if (img.data('yelp-load-bound') == true)
      img.unbind('load', fig.yelp_auto_resize);
    if (!imgs[i].complete) {
      img.data('yelp-load-bound', true);
      img.bind('load', fig.yelp_auto_resize);
      return false;
    }
  }
  $(window).unbind('resize', yelp_resize_imgs);
  var zoom = fig.children('div.inner').children('a.zoom');
  if (fig.find('div.contents:first').is(':hidden')) {
    zoom.hide();
    return;
  }
  for (var i = 0; i < imgs.length; i++) {
    var img = $(imgs[i]);
    if (img.data('yelp-original-width') == undefined) {
      var iwidth = parseInt(img.attr('width'));
      if (!iwidth)
        iwidth = img[0].width;
      img.data('yelp-original-width', iwidth);
      var iheight = parseInt(img.attr('height'));
      if (!iheight)
        iheight = img[0].height * (iwidth / img[0].width);
      img.data('yelp-original-height', iheight);
    }
    if (img.data('yelp-original-width') > img.parent().width()) {
      if (img.data('yelp-zoomed') != true) {
        img[0].width = img.parent().width();
        img[0].height = (parseInt(img.data('yelp-original-height')) *
                         img.width() / parseInt(img.data('yelp-original-width')));
      }
      zoom.show();
    }
    else {
      img[0].width = img.data('yelp-original-width');
      img[0].height = img.data('yelp-original-height');
      zoom.hide();
    }
  }
  /* The image scaling above can cause the window to resize if it causes
   * scrollbars to disappear or reapper. Unbind the resize handler before
   * scaling the image. Don't rebind immediately, because we'll still get
   * that resize event in an idle. Rebind on the callback instead.
   */
  var reresize = function () {
    $(window).unbind('resize', reresize);
    $(window).bind('resize', yelp_resize_imgs);
  }
  $(window).bind('resize', reresize);
  return false;
};
yelp_resize_imgs = function () {
  $('div.figure img').parents('div.figure').each(function () {
    var div = $(this);
    if (div.data('yelp-zoom-timeout') == undefined)
      div.data('yelp-zoom-timeout', setTimeout(function () { div.yelp_auto_resize() }, 1));
  });
  return false;
};
$(document).ready(function () {
  $('div.figure img').parents('div.figure').each(function () {
    var fig = $(this);
    var zoom = fig.children('div.inner').children('a.zoom');
    zoom.append($('<canvas width="10" height="10"/>'));
    yelp_paint_zoom(zoom, false);
    zoom.data('yelp-zoomed', false);
    zoom.click(function () {
      var zoomed = !zoom.data('yelp-zoomed');
      zoom.data('yelp-zoomed', zoomed);
      zoom.parent().find('img').each(function () {
        var zimg = $(this);
        zimg.data('yelp-zoomed', zoomed);
        if (zoomed) {
          zimg[0].width = zimg.data('yelp-original-width');
          zimg[0].height = zimg.data('yelp-original-height');
        } else {
          zimg[0].width = zimg.parent().width();
          zimg[0].height = (parseInt(zimg.data('yelp-original-height')) *
                            zimg.width() / parseInt(zimg.data('yelp-original-width')));
        }
        yelp_paint_zoom(zoom, zoomed);
      });
      return false;
    });
  });
  yelp_resize_imgs();
  $(window).bind('resize', yelp_resize_imgs);
});
function yelp_init_video (element) {
  var video = $(element);
  video.removeAttr('controls');

  var controls = $('<div class="media-controls media-controls-' + element.nodeName + '"></div>');
  var playControl = $('<button class="media-play"></button>').attr({
    'data-play-label': video.attr('data-play-label'),
    'data-pause-label': video.attr('data-pause-label'),
    'value': video.attr('data-play-label')
  });
  var playCanvas = $('<canvas width="20" height="20"></canvas>');
  playControl.append(playCanvas);
  var rangeCanvas = $('<canvas width="104" height="20"></canvas>');
  var rangeCanvasCtxt = rangeCanvas[0].getContext('2d');
  rangeCanvasCtxt.strokeStyle = yelp_color_gray_border;
  rangeCanvasCtxt.strokeWidth = 1;
  rangeCanvasCtxt.strokeRect(0.5, 0.5, 103, 19);
  var currentSpan = $('<span class="media-current">0:00</span>');
  var durationSpan = $('<span class="media-duration">-:--</span>');
  controls.append(playControl,
                  $('<div class="media-range"></div>').append(rangeCanvas),
                  $('<div class="media-time"></div>').append(currentSpan, durationSpan));
  video.after(controls);

  var playCanvasCtxt = playCanvas[0].getContext('2d');
  var paintPlayButton = function () {
    playCanvasCtxt.fillStyle = yelp_color_gray_background;
    playCanvasCtxt.clearRect(0, 0, 20, 20);
    playCanvasCtxt.beginPath();
    playCanvasCtxt.moveTo(5, 5);   playCanvasCtxt.lineTo(5, 15);
    playCanvasCtxt.lineTo(15, 10); playCanvasCtxt.lineTo(5, 5);
    playCanvasCtxt.fill();
  }
  var paintPauseButton = function () {
    playCanvasCtxt.fillStyle = yelp_color_gray_background;
    playCanvasCtxt.clearRect(0, 0, 20, 20);
    playCanvasCtxt.beginPath();
    playCanvasCtxt.moveTo(5, 5);   playCanvasCtxt.lineTo(9, 5);
    playCanvasCtxt.lineTo(9, 15);  playCanvasCtxt.lineTo(5, 15);
    playCanvasCtxt.lineTo(5, 5);   playCanvasCtxt.fill();
    playCanvasCtxt.beginPath();
    playCanvasCtxt.moveTo(11, 5);  playCanvasCtxt.lineTo(15, 5);
    playCanvasCtxt.lineTo(15, 15); playCanvasCtxt.lineTo(11, 15);
    playCanvasCtxt.lineTo(11, 5);  playCanvasCtxt.fill();
  }
  paintPlayButton();

  var video_el = video[0];
  var mediaChange = function () {
    if (video_el.ended)
      video_el.pause()
    if (video_el.paused) {
      playControl.attr('value', playControl.attr('data-play-label'));
      paintPlayButton();
    }
    else {
      playControl.attr('value', playControl.attr('data-pause-label'));
      paintPauseButton();
    }
  }
  video_el.addEventListener('play', mediaChange, false);
  video_el.addEventListener('pause', mediaChange, false);
  video_el.addEventListener('ended', mediaChange, false);

  var playClick = function () {
    if (video_el.paused || video_el.ended)
      video_el.play();
    else
      video_el.pause();
  };
  playControl.click(playClick);

  var ttmlDiv = video.parent().children('div.media-ttml');
  var ttmlNodes = ttmlDiv.find('.media-ttml-node');

  var timeUpdate = function () {
    var pct = (element.currentTime / element.duration) * 100;
    rangeCanvasCtxt.fillStyle = yelp_color_gray_border;
    rangeCanvasCtxt.clearRect(2, 2, 100, 16);
    rangeCanvasCtxt.fillRect(2, 2, pct, 16);
    var mins = parseInt(element.currentTime / 60);
    var secs = parseInt(element.currentTime - (60 * mins))
    currentSpan.text(mins + (secs < 10 ? ':0' : ':') + secs);
    ttmlNodes.each(function () {
      var ttml = this;
      if (element.currentTime >= parseFloat(ttml.getAttribute('data-ttml-begin')) &&
          (!ttml.hasAttribute('data-ttml-end') ||
           element.currentTime < parseFloat(ttml.getAttribute('data-ttml-end')) )) {
        if (ttml.tagName == 'span' || ttml.tagName == 'SPAN')
          ttml.style.display = 'inline';
        else
          ttml.style.display = 'block';
      }
      else {
        ttml.style.display = 'none';
      }
    });
  };
  element.addEventListener('timeupdate', timeUpdate, false);
  var durationUpdate = function () {
    if (!isNaN(element.duration)) {
      mins = parseInt(element.duration / 60);
      secs = parseInt(element.duration - (60 * mins));
      durationSpan.text(mins + (secs < 10 ? ':0' : ':') + secs);
    }
  };
  element.addEventListener('durationchange', durationUpdate, false);

  rangeCanvas.click(function (event) {
    var pct = event.clientX - Math.floor(rangeCanvas.offset().left);
    if (pct < 0)
      pct = 0;
    if (pct > 100)
      pct = 100;
    element.currentTime = (pct / 100.0) * element.duration;
  });
};
$(document).ready(function () {
  $('video, audio').each(function () { yelp_init_video(this) });;
});
]]></xsl:text>
</xsl:template>


<!--**==========================================================================
html.js.syntax
Output JavaScript for syntax highlighting
:Revision: version="1.0" date="2011-04-16" status="final"
$node: The node to create JavaScript for.

This template outputs JavaScript that does syntax highlighting. JavaScript
@{html.syntax.highlight} is #{true}. Note that this content just initializes
the syntax highlighting. The real work is done by #{jquery.syntax.js}, which
is included by *{html.js.syntax}.
-->
<xsl:template name="html.js.syntax">
  <xsl:param name="node" select="."/>
  <xsl:if test="$html.syntax.highlight">
<xsl:text><![CDATA[
$(document).ready( function () { jQuery.syntax({root: ']]></xsl:text>
<xsl:value-of select="$html.js.root"/><xsl:text><![CDATA[', blockLayout: 'yelp',
theme: false, linkify: false}); });
]]></xsl:text>
  </xsl:if>
</xsl:template>


<!--%%==========================================================================
html.js.mode
Output JavaScript specific to the input format.
:Revision:version="1.0" date="2010-01-01" status="final"

This template is called by *{html.js.content} to output JavaScript specific to
the input format. Importing stylesheets may implement this for any element that
will be passed to *{html.page}. If they do not, the output HTML will only have
the common JavaScript.
-->
<xsl:template mode="html.js.mode" match="*">
</xsl:template>


<!--**==========================================================================
html.js.custom
Stub to reference custom JavaScript common to all HTML transformations.
:Stub: true
:Revision: version="1.0" date="2010-04-16" status="final"
$node: The node to create JavaScript for.

This template is a stub, called by *{html.js}. You can override this template
to reference additional external JavaScript files. If you want to include
JavaScript into the main #{script} tag instead, use *{html.js.content.custom}.
-->
<xsl:template name="html.js.custom">
  <xsl:param name="node" select="."/>
</xsl:template>


<!--**==========================================================================
html.js.content.custom
Stub to output custom JavaScript common to all HTML transformations.
:Stub: true
:Revision: version="1.0" date="2010-04-16" status="final"
$node: The node to create JavaScript for.

This template is a stub, called by *{html.js.content}. You can override this
template to provide additional JavaScript that will be used by all HTML output.
This template is called inside a #{script} tag, and is intended to include
JavaScript code in the output page. See *{html.js.custom} to include a custom
reference to an external JavaScript file.
-->
<xsl:template name="html.js.content.custom">
  <xsl:param name="node" select="."/>
</xsl:template>


<!--**==========================================================================
html.lang.attrs
Output #{lang} and #{dir} attributes.
:Revision: version="1.0" date="2010-06-10" status="final"
$node: The current element in the input document.
$parent: A parent node to take ${lang} and ${dir} from.
$lang: The language for ${node}.
$dir: The text directionality for ${node}.

This template outputs #{lang}, #{xml:lang}, or #{dir} attributes if necessary.
If ${lang} is not set, it will be taken from the #{xml:lang} or #{lang}
attribute of ${node}. If ${dir} is not set, it will be taken from the #{its:dir}
attribute of ${node} or computed based on ${lang}.

The ${parent} parameter defaults to an empty node set. If it is set to a
non-empty node set, this template will attempt to get ${lang} and ${dir} from
${parent} if they are not set on ${node}. This is occasionally useful when a
wrapper element in a source language doesn't directly create any output
elements.

This template outputs either an #{xml:lang} or a #{lang} attribute, depending
on whether @{html.xhtml} is #{true}. It only outputs an #{xml:lang} or #{lang}
attribute if $lang is non-empty. This template also outputs a #{dir} attribute
if ${dir} is non-empty.
-->
<xsl:template name="html.lang.attrs">
  <xsl:param name="node" select="."/>
  <xsl:param name="parent" select="/false"/>
  <xsl:param name="lang">
    <xsl:choose>
      <xsl:when test="$node/@xml:lang">
        <xsl:value-of select="$node/@xml:lang"/>
      </xsl:when>
      <xsl:when test="$node/@lang">
        <xsl:value-of select="$node/@lang"/>
      </xsl:when>
      <xsl:when test="$parent/@xml:lang">
        <xsl:value-of select="$parent/@xml:lang"/>
      </xsl:when>
      <xsl:when test="$parent/@lang">
        <xsl:value-of select="$parent/@lang"/>
      </xsl:when>
    </xsl:choose>
  </xsl:param>
  <xsl:param name="dir">
    <xsl:choose>
      <xsl:when test="$node/@its:dir">
        <xsl:value-of select="$node/@its:dir"/>
      </xsl:when>
      <xsl:when test="($node/@xml:lang or $node/@lang) and (string($lang) != '')">
        <xsl:call-template name="l10n.direction">
          <xsl:with-param name="lang" select="$lang"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$parent/@its:dir">
        <xsl:value-of select="$node/@its:dir"/>
      </xsl:when>
      <xsl:when test="string($lang) != ''">
        <xsl:call-template name="l10n.direction">
          <xsl:with-param name="lang" select="$lang"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:param>
  <xsl:if test="string($lang) != ''">
    <xsl:choose>
      <xsl:when test="$html.xhtml">
        <xsl:attribute name="xml:lang">
          <xsl:value-of select="$lang"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
        <xsl:attribute name="lang">
          <xsl:value-of select="$lang"/>
        </xsl:attribute>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:if>
  <xsl:if test="string($dir) != ''">
    <xsl:attribute name="dir">
      <xsl:value-of select="$dir"/>
    </xsl:attribute>
  </xsl:if>
</xsl:template>


<!--**==========================================================================
html.syntax.class
Output HTML class values for syntax highlighting.
:Revision:version="3.12" date="2013-11-02" status="final"
$node: The source element whose content will be syntax highlighted.
$mime: A MIME type identifying the content, as from a Mallard #{mime} attribute.
$language: A name identifying the content as from a DocBook #{language} attribute.

This template looks at ${mime} and ${language} and determines if there is a
suitable syntax highlighting brush available. If so, it outputs a string that
should be placed in the #{class} attribute of a #{pre} element by the calling
template.
-->
<xsl:template name="html.syntax.class">
  <xsl:param name="node" select="."/>
  <xsl:param name="mime" select="$node/@mime"/>
  <xsl:param name="language" select="$node/@language"/>
  <xsl:choose>
    <xsl:when test="$mime = 'application/x-shellscript' or $language = 'bash'">
      <xsl:text>syntax brush-bash-script</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'text/x-csrc' or $mime = 'text/x-chdr' or
                    $mime = 'text/x-c++hdr' or $mime = 'text/x-c++src' or
                    $mime = 'text/x-objcsrc' or $language = 'c' or
                    $language = 'cpp' or $language = 'objc'">
      <xsl:text>syntax brush-clang</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'text/x-csharp' or $language = 'csharp'">
      <xsl:text>syntax brush-csharp</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'text/css' or $language = 'css'">
      <xsl:text>syntax brush-css</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'text/x-patch' or $language = 'diff'">
      <xsl:text>syntax brush-diff</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'text/html' or $mime = 'application/xml' or
                    substring($mime, string-length($mime) - 3) = '+xml' or
                    $language = 'html' or $language = 'xml'">
      <xsl:text>syntax brush-html</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'text/x-java' or $language = 'java'">
      <xsl:text>syntax brush-java</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'application/javascript' or $language = 'javascript'">
      <xsl:text>syntax brush-javascript</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'text/x-scheme' or $mime = 'text/x-emacs-lisp' or
                    $language = 'lisp'">
      <xsl:text>syntax brush-lisp</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'text/x-lua' or $language = 'lua'">
      <xsl:text>syntax brush-lua</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'text/x-pascal' or $language = 'pascal'">
      <xsl:text>syntax brush-pascal</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'application/x-perl' or $language = 'perl'">
      <xsl:text>syntax brush-perl5</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'application/x-php' or $language = 'php'">
      <xsl:text>syntax brush-php-script</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'text/x-python' or $language = 'python'">
      <xsl:text>syntax brush-python</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'application/x-ruby' or $language = 'ruby'">
      <xsl:text>syntax brush-ruby</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'text/x-sql' or $language = 'sql'">
      <xsl:text>syntax brush-sql</xsl:text>
    </xsl:when>
    <xsl:when test="$mime = 'application/x-yaml' or $language = 'yaml'">
      <xsl:text>syntax brush-yaml</xsl:text>
    </xsl:when>
  </xsl:choose>
</xsl:template>

</xsl:stylesheet>
                                                                                                                                                                                                                                                                                                                                                                                       obj-y += clk-mtk.o clk-pll.o clk-gate.o clk-apmixed.o
obj-$(CONFIG_RESET_CONTROLLER) += reset.o
obj-y += clk-mt8135.o
obj-y += clk-mt8173.o
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    <?xml version='1.0' encoding='UTF-8'?><!-- -*- indent-tabs-mode: nil -*- -->
<!--
This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License
along with this program; see the file COPYING.LGPL.  If not, see <http://www.gnu.org/licenses/>.
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

<!--!!==========================================================================
Icons
Specify common named icons to style output.
:Requires: l10n
:Revision:version="1.0" date="2010-05-25" status="final"

This stylesheet provides a common interface to specify icons for transformations
to presentation-oreinted formats. This allows similar output for different
types of input documents.
-->


<!--@@==========================================================================
icons.base_url
The default URL prefix for all icons.
:Revision:version="1.0" date="2010-01-26" status="final"

This parameter provides a default URL prefix. All icon locations in this
stylesheet reference files directly under this URL prefix by default. If
you override the individual icon parameters in this stylesheet, this parameter
has no effect. This parameter should end with a trailing slash.
-->
<xsl:param name="icons.base_url" select="''"/>


<!--@@==========================================================================
icons.size.note
The size of the note icons.
:Revision:version="1.0" date="2010-03-05" status="final"

This parameter specifies the size of the note icons. Use an integer giving
the width of the image files in pixels. Icons are assumed to be square, and
all note icons are assumed to have the same size.
-->
<xsl:param name="icons.size.note" select="24"/>


<!--@@==========================================================================
icons.note
The URL for the note icon.
:Revision:version="1.0" date="2010-05-03" status="final"

This parameter specifies the URL for the icon used for regular notes.
-->
<xsl:param name="icons.note"
           select="concat($icons.base_url, 'yelp-note.png')"/>


<!--@@==========================================================================
icons.note.bug
The URL for the bug note icon.
:Revision:version="1.0" date="2010-05-03" status="final"

This parameter specifies the URL for the icon used for bug notes.
-->
<xsl:param name="icons.note.bug"
           select="concat($icons.base_url, 'yelp-note-bug.png')"/>


<!--@@==========================================================================
icons.note.important
The URL for the important note icon.
:Revision:version="1.0" date="2010-05-03" status="final"

This parameter specifies the URL for the icon used for important notes.
-->
<xsl:param name="icons.note.important"
           select="concat($icons.base_url, 'yelp-note-important.png')"/>


<!--@@==========================================================================
icons.note.tip
The URL for the tip note icon.
:Revision:version="1.0" date="2010-05-03" status="final"

This parameter specifies the URL for the icon used for tip notes.
-->
<xsl:param name="icons.note.tip"
           select="concat($icons.base_url, 'yelp-note-tip.png')"/>


<!--@@==========================================================================
icons.note.warning
The URL for the warning note icon.
:Revision:version="1.0" date="2010-05-03" status="final"

This parameter specifies the URL for the icon used for warning notes.
-->
<xsl:param name="icons.note.warning"
           select="concat($icons.base_url, 'yelp-note-warning.png')"/>


<!--@@==========================================================================
icons.size.quote
The size of the quote icons.
:Revision:version="3.8" date="2012-09-29" status="final"

This parameter specifies the size of the block quote icon. Use an integer giving
the width of the image files in pixels. Icons are assumed to be square, and all
quote icons are assumed to have the same size.

As of version 3.8, there is no longer an actual image file for the quote icon.
Instead, a language-appropriate quotation character is placed in the margin.
This parameters still affects the size of that character.
-->
<xsl:param name="icons.size.quote" select="48"/>


</xsl:stylesheet>
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        <?xml version='1.0' encoding='UTF-8'?><!-- -*- indent-tabs-mode: nil -*- -->
<!--
This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License
along with this program; see the file COPYING.LGPL.  If not, see <http://www.gnu.org/licenses/>.
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

<!--!!==========================================================================
Utilities
Common XSLT Utilities.
:Revision:version="1.0" date="2010-05-25" status="final"

This stylesheet contains various useful utilities that are used throughout
the Yelp stylesheets.
-->


<!--**==========================================================================
utils.repeat_string
Repeat a string a given number of times.
:Revision:version="1.0" date="2011-08-24" status="final"
$string: The string to repeat.
$number: The number of times to repeat ${string}.

This template repeats the ${string} argument ${number} times.
-->
<xsl:template name="utils.repeat_string">
  <xsl:param name="string" select="''"/>
  <xsl:param name="number" select="0"/>
  <xsl:if test="$number &gt; 0">
    <xsl:value-of select="$string"/>
    <xsl:call-template name="utils.repeat_string">
      <xsl:with-param name="string" select="$string"/>
      <xsl:with-param name="number" select="$number - 1"/>
    </xsl:call-template>
  </xsl:if>
</xsl:template>


<!--**==========================================================================
utils.strip_newlines
Strip leading or trailing newlines from a string.
:Revision:version="1.0" date="2010-05-25" status="final"
$string: The string to strip newlines from.
$leading: Whether to strip leading newlines.
$trailing: Whether to strip trailing newlines.

This template strips at most one leading and one trailing newline from
${string}.  This is useful for preformatted block elements where leading and
trailing newlines are ignored to make source formatting easier for authors.
-->
<xsl:template name="utils.strip_newlines">
  <xsl:param name="string"/>
  <xsl:param name="leading" select="false()"/>
  <xsl:param name="trailing" select="false()"/>
  <xsl:choose>
    <xsl:when test="$leading">
      <xsl:variable name="new">
        <xsl:choose>
          <xsl:when test="starts-with($string, '&#x000A;')">
            <xsl:value-of select="substring($string, 2)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$string"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="$trailing">
          <xsl:call-template name="utils.strip_newlines">
            <xsl:with-param name="string" select="$new"/>
            <xsl:with-param name="leading" select="false()"/>
            <xsl:with-param name="trailing" select="true()"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$new"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:when>
    <xsl:when test="$trailing">
      <xsl:choose>
        <xsl:when test="substring($string, string-length($string)) = '&#x000A;'">
          <xsl:value-of select="substring($string, 1, string-length($string) - 1 )"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$string"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:when>
  </xsl:choose>
</xsl:template>


<!--**==========================================================================
utils.linenumbering
Number each line in a verbatim environment.
:Revision:version="1.0" date="2010-12-03" status="final"
$node: The verbatim element to create the line numbering for.
$number: The starting line number.

This template outputs a string with line numbers for each line in a verbatim
elements.  Each line number is on its own line, allowing the output string to
be placed to the side of the verbatim output.
-->
<xsl:template name="utils.linenumbering">
  <xsl:param name="node" select="."/>
  <xsl:param name="number" select="1"/>
  <xsl:param name="string">
    <xsl:choose>
      <xsl:when test="$node/node()[1]/self::text() and starts-with($node/node()[1], '&#x000A;')">
        <xsl:value-of select="substring-after(string($node), '&#x000A;')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="string($node)"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:param>
  <xsl:choose>
    <xsl:when test="contains($string, '&#x000A;')">
      <xsl:number value="$number"/>
      <xsl:text>&#x000A;</xsl:text>
      <xsl:call-template name="utils.linenumbering">
        <xsl:with-param name="node" select="$node"/>
        <xsl:with-param name="number" select="$number + 1"/>
        <xsl:with-param name="string"
                        select="substring-after($string, '&#x000A;')"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="string-length($string) != 0">
      <xsl:number value="$number"/>
      <xsl:text>&#x000A;</xsl:text>
    </xsl:when>
  </xsl:choose>
</xsl:template>


<!--**==========================================================================
utils.email_address
Get an email address from a mailto URL.
:Revision:version="3.4" date="2012-01-18" status="final"
$href: The mailto URL.

This template takes a mailto URL and returns an email address, stripping the
URL scheme as well as any query string.
-->
<xsl:template name="utils.email_address">
  <xsl:param name="url"/>
  <xsl:variable name="addy">
    <xsl:value-of select="substring-after($url, 'mailto:')"/>
  </xsl:variable>
  <xsl:choose>
    <xsl:when test="contains($addy, '?')">
      <xsl:value-of select="substring-before($addy, '?')"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="$addy"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

</xsl:stylesheet>
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             <?xml version='1.0' encoding='UTF-8'?><!-- -*- indent-tabs-mode: nil -*- -->
<!--
This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License
along with this program; see the file COPYING.LGPL.  If not, see
<http://www.gnu.org/licenses/>.
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:math="http://exslt.org/math"
                extension-element-prefixes="math"
                version="1.0">

<!--!!==========================================================================
Localized Numbers
-->


<!--**==========================================================================
l10n.number
Formats a number according to a localized numbering system
$value: The numeric value of the number to format
$format: The numbering system to use

REMARK: Talk about numbering systems
-->
<xsl:template name="l10n.number">
  <xsl:param name="value"/>
  <xsl:param name="format"/>
  <xsl:choose>
    <xsl:when test="$format='decimal' or $format='1'">
      <xsl:number format="1" value="$value"/>
    </xsl:when>
    <xsl:when test="$format='alpha-lower' or $format='a'">
      <xsl:number format="a" value="$value"/>
    </xsl:when>
    <xsl:when test="$format='alpha-upper' or $format='A'">
      <xsl:number format="A" value="$value"/>
    </xsl:when>
    <xsl:when test="$format='roman-lower' or $format='i'">
      <xsl:number format="i" value="$value"/>
    </xsl:when>
    <xsl:when test="$format='roman-upper' or $format='I'">
      <xsl:number format="I" value="$value"/>
    </xsl:when>

    <xsl:when test="$format='cjk-japanese'     or
                    $format='cjk-chinese-simp' or
                    $format='cjk-chinese-trad' ">
      <xsl:call-template name="l10n.number.cjk-ideographic">
        <xsl:with-param name="value" select="$value"/>
        <xsl:with-param name="format" select="$format"/>
      </xsl:call-template>
    </xsl:when>

    <xsl:when test="$format='ionic-lower' or $format='ionic-upper'">
      <xsl:call-template name="l10n.number.ionic">
        <xsl:with-param name="value" select="$value"/>
        <xsl:with-param name="format" select="$format"/>
      </xsl:call-template>
    </xsl:when>

    <xsl:when test="$format='decimal-arabic'">
      <xsl:call-template name="l10n.number.numeric">
        <xsl:with-param name="value" select="$value"/>
        <xsl:with-param name="digits" select="'٠١٢٣٤٥٦٧٨٩'"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="$format='decimal-persian'">
      <xsl:call-template name="l10n.number.numeric">
        <xsl:with-param name="value" select="$value"/>
        <xsl:with-param name="digits" select="'۰۱۲۳۴۵۶۷۸۹'"/>
      </xsl:call-template>
    </xsl:when>

    <xsl:when test="$format='alpha-serbian-lower'">
      <xsl:call-template name="l10n.number.alphabetic">
        <xsl:with-param name="value" select="$value"/>
        <xsl:with-param name="alphabet"
                        select="'абвгдђежзијклљмнњопрстћуфхцчџш'"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="$format='alpha-serbian-upper'">
      <xsl:call-template name="l10n.number.alphabetic">
        <xsl:with-param name="value" select="$value"/>
        <xsl:with-param name="alphabet"
                        select="'АБВГДЂЕЖЗИЈКЛЉМНЊОПРСТЋУФХЦЧЏШ'"/>
      </xsl:call-template>
    </xsl:when>

    <xsl:when test="$format='alpha-thai'">
      <xsl:call-template name="l10n.number.alphabetic">
        <xsl:with-param name="value" select="$value"/>
        <xsl:with-param name="alphabet"
                        select="'กขคงจฉชซ�